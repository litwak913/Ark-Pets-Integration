use anyhow::{Context, Result};

#[cfg(target_os = "macos")]
use objc2_core_foundation::{
    kCFAllocatorDefault, kCFRunLoopDefaultMode, CFRunLoop, CFRunLoopRunResult, CFRunLoopTimer,
};

pub fn reset_signal() -> Result<()> {
    #[cfg(target_family = "unix")]
    {
        use anyhow::bail;
        use libc::c_int;

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

        reset_signal_handler(libc::SIGBUS)?;
        reset_signal_handler(libc::SIGSEGV)?;
        reset_signal_handler(libc::SIGINT)?;
    }
    Ok(())
}

pub fn open_console() -> Result<()> {
    #[cfg(target_family = "windows")]
    {
        use windows_sys::Win32::System::Console::AllocConsole;
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
            use std::ffi::c_void;
            use std::ptr;

            extern "C-unwind" fn noop_timer(_timer: *mut CFRunLoopTimer, _void: *mut c_void) {}
            let timer;
            timer = CFRunLoopTimer::new(
                kCFAllocatorDefault,
                1e20,
                0.0,
                0,
                0,
                Some(noop_timer),
                ptr::null_mut(),
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
