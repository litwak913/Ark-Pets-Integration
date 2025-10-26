use std::path::PathBuf;

use anyhow::{anyhow, Context, Result};
use native_dialog::{DialogBuilder, MessageLevel};

#[cfg(target_family = "windows")]
use windows_sys::Win32::System::Console::AllocConsole;

#[cfg(target_family = "unix")]
use anyhow::bail;
#[cfg(target_family = "unix")]
use libc::c_int;

#[cfg(target_os = "macos")]
use objc2_core_foundation::{
    kCFAllocatorDefault, kCFRunLoopDefaultMode, CFRunLoop, CFRunLoopRunResult, CFRunLoopTimer,
};
#[cfg(target_os = "macos")]
use std::ptr;

pub fn reset_signal() -> Result<()> {
    #[cfg(target_family = "unix")]
    {
        reset_signal_handler(libc::SIGBUS)?;
        reset_signal_handler(libc::SIGSEGV)?;
        reset_signal_handler(libc::SIGINT)?;
    }
    Ok(())
}

#[cfg(target_family = "unix")]
fn reset_signal_handler(signal: c_int) -> Result<()> {
    unsafe {
        let mut action: libc::sigaction = std::mem::zeroed();
        action.sa_sigaction = libc::SIG_DFL;
        match libc::sigaction(signal, &action, std::ptr::null_mut()) {
            0 => Ok(()),
            _ => bail!("sigaction({}): {}", signal, std::io::Error::last_os_error()),
        }
    }
}

pub fn open_console() -> Result<()> {
    #[cfg(target_family = "windows")]
    {
        unsafe {
            AllocConsole();
        }
    }
    Ok(())
}

pub fn start_cocoa_thread() -> Result<()> {
    #[cfg(target_os = "macos")]
    {
        unsafe {
            let timer;
            timer = CFRunLoopTimer::new(
                kCFAllocatorDefault,
                1e20,
                0.0,
                0,
                0,
                Some(|timer, void| {}),
                *ptr::null(),
            )
            .with_context(|| "Failed to create timer")?;
            let current = CFRunLoop::current().with_context(|| "Failed to get mainloop")?;
            current.add_timer(Some(&*timer), kCFRunLoopDefaultMode);
            loop {
                if CFRunLoop::run_in_mode(kCFRunLoopDefaultMode, 1e20, false)
                    == CFRunLoopRunResult::Finished
                {
                    break;
                }
            }
        }
    }

    Ok(())
}

pub fn show_err(msg: String) {
    DialogBuilder::message()
        .set_level(MessageLevel::Error)
        .set_title("Start Failed")
        .set_text(msg.as_str())
        .alert()
        .show()
        .unwrap_or_default()
}

pub fn get_jar_list(path: PathBuf) -> Result<Vec<PathBuf>> {
    let dirs = path
        .read_dir()
        .with_context(|| format!("Failed to read dir: {}", path.display()))?;

    let paths = dirs
        .filter_map(|res| res.ok())
        .filter(|file| !file.path().is_dir())
        .map(|file| file.path());

    let jars = paths.filter(|file| match file.extension() {
        Some(ext) => ext == "jar",
        None => false,
    });
    Ok(jars.collect())
}

pub fn build_path(base: &PathBuf, segments: &[&str]) -> PathBuf {
    let mut path = base.clone();
    segments.iter().for_each(|s| path.push(s));
    path
}

pub fn paths_to_strs(paths: &[PathBuf]) -> Result<Vec<&str>> {
    paths
        .iter()
        .map(|p| {
            p.to_str()
                .ok_or_else(|| anyhow!("Invalid UTF-8 path: {:?}", p))
        })
        .collect()
}
