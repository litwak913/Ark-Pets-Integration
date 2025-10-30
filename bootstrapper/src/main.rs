#![windows_subsystem = "windows"]

pub mod config;
pub mod consts;
pub mod jni_utils;
pub mod logger;
pub mod platform;
pub mod utils;

use crate::config::Config;
use crate::consts::{LIBS_DIR, PLUGINS_DIR};
use crate::jni_utils::{build_args_array, find_libjvm, set_thread_class_loader};
use crate::utils::{build_path, get_jar_list, paths_to_strs, show_err};

#[cfg(target_os = "windows")]
use crate::platform::open_console;
#[cfg(target_family = "unix")]
use crate::platform::reset_signal;
#[cfg(target_os = "macos")]
use crate::platform::start_cocoa_thread;

use anyhow::{Context, Result};
use jni::{
    objects::{JObject, JValueGen},
    InitArgsBuilder, JavaVM,
};
use log::{debug, error, info, warn};
use std::process::exit;
#[cfg(target_os = "macos")]
use std::thread;
use std::{
    env::{current_dir, current_exe, set_current_dir},
    fs::{create_dir, File},
    io::Read,
    path::PathBuf,
};

const LAUNCHER_VERSION: i32 = 1;

#[cfg(debug_assertions)]
const APP_DIR: &str = "/Applications/ArkPets.app";
#[cfg(not(debug_assertions))]
const APP_DIR: &str = "";

fn main() {
    if cfg!(debug_assertions) {
        let _ = set_current_dir(APP_DIR);
    }
    if let Ok(()) = logger::init(log::LevelFilter::Debug) {
        cfg_if::cfg_if! {
            if #[cfg(target_os="macos")] {
                info!("Starting Worker Thread");
                thread::spawn(|| {
                    if let Err(err) = launcher_main() {
                        error!("{:?}", err);
                        show_err(format!("{:?}", err));
                        exit(-1);
                    }
                });
                info!("Starting Cocoa Thread");
                if let Err(err) = start_cocoa_thread() {
                    error!("{:?}", err);
                    show_err(format!("{:?}", err));
                }
            } else {
                if let Err(err) = launcher_main() {
                    error!("{:?}", err);
                    show_err(format!("{:?}", err));
                    exit(-1)
                }
            }
        }
    } else {
        show_err("Failed to init logger".to_string());
        exit(-1)
    }
    exit(0)
}

fn launcher_main() -> Result<()> {
    // 1. Init Launcher
    #[cfg(all(debug_assertions, target_os = "windows"))]
    open_console()?;

    info!("ArkPets Bootstrapper V{}", LAUNCHER_VERSION);
    let (current_work, app_dir) = init_launcher()?;
    info!("Current working dir is {}", current_work.display());
    info!("Launcher dir is {}", app_dir.display());

    // 2. Load Config
    let config = load_config(&app_dir)?;

    // 3. Prepare environment
    // 3.1. Platform
    cfg_if::cfg_if! {
        if #[cfg(target_family = "unix")] {
            debug!("Reset single handlers");
            reset_signal().with_context(|| "Cannot reset single handlers")?;
        } else if #[cfg(all(target_os = "windows", not(debug_assertions)))] {
            open_console().with_context(|| "Failed to open debug console")?;
        }
    }
    if config.runtime.use_user_data {
        let mut user_data =
            dirs::data_local_dir().with_context(|| "Cannot determine user data path")?;
        user_data.push("ArkPets");
        if !user_data.exists() {
            create_dir(&user_data).with_context(|| "Cannot create user data path")?;
        }
        info!("Setting working dir to {}", &user_data.display());
        set_current_dir(user_data).with_context(|| "Failed to set working dir")?;
    }
    // 3.2. Dirs and Jars
    info!("Searching main and plugin jars");
    let jars = search_build_jars(&app_dir, &config)?;
    debug!("Jar list: {}", jars);

    // 4. Init the JVM
    info!("Initializing the JVM");
    let mut jvm_arg_builder = InitArgsBuilder::new();
    jvm_arg_builder
        .try_option(format!("-Djava.class.path={}", jars))
        .with_context(|| "Failed to parse jar args.")?;
    for arg in config.runtime.jvm_args {
        jvm_arg_builder
            .try_option(arg)
            .with_context(|| "Failed to parse JVM args")?;
    }
    let libjvm_path = find_libjvm(&app_dir, config.runtime.use_local_jvm);
    debug!("JVM Library path {}", libjvm_path.display());
    let jvm_arg = jvm_arg_builder
        .build()
        .with_context(|| "Failed to build JVM args.")?;
    let jvm = JavaVM::with_libjvm(jvm_arg, || Ok(libjvm_path))
        .with_context(|| "Failed to launch JVM.")?;

    // 5. Find main class and prepare JVM environment
    info!("Preparing the JVM environment");
    let main_class_name = config.arkpets.main.replace(".", "/");
    let mut env = jvm.attach_current_thread()?;
    let ap_main_class = env
        .find_class(&main_class_name)
        .with_context(|| "Cannot find main class")?;
    let ap_args = build_args_array(config.arkpets.args, &mut env)?;
    set_thread_class_loader(&mut env, &ap_main_class)?;

    // 6. It's Goldenglow time!
    info!("It's Goldenglow time!");
    env.call_static_method(
        ap_main_class,
        "main",
        "([Ljava/lang/String;)V",
        &[JValueGen::Object(&JObject::from(ap_args))],
    )?
    .v()?;
    Ok(())
}

fn init_launcher() -> Result<(PathBuf, PathBuf)> {
    let current_work = current_dir().with_context(|| "Cannot determine working path.")?;
    let mut app_dir = if cfg!(debug_assertions) {
        PathBuf::from(APP_DIR)
    } else {
        current_exe().with_context(|| "Cannot determine application path.")?
    };
    if cfg!(not(debug_assertions)) {
        app_dir.pop();
    }
    if cfg!(target_os = "macos") {
        app_dir.pop(); // app bundle root
    }
    Ok((current_work, app_dir))
}

fn load_config(app_dir: &PathBuf) -> Result<Config> {
    let toml_path = app_dir.join("launch.toml");
    info!("Loading config {}", toml_path.display());
    let mut toml_reader = File::open(&toml_path)
        .with_context(|| format!("Cannot open config {}", toml_path.display()))?;
    let mut toml_content: String = String::new();
    toml_reader
        .read_to_string(&mut toml_content)
        .with_context(|| "Cannot read config")?;
    toml::from_str(&toml_content).with_context(|| "Cannot parse config toml")
}

fn search_build_jars(app_dir: &PathBuf, config: &Config) -> Result<String> {
    debug!("Searching for the main jar");
    let main_jar_path = build_path(
        app_dir,
        &[LIBS_DIR, &*format!("desktop-{}.jar", config.arkpets.ver)],
    );
    debug!("Searching for the plugin jars");
    let plugins_lib_path = build_path(app_dir, &[PLUGINS_DIR]);
    if !plugins_lib_path.exists() {
        warn!("Plugins directory doesn't exist, creating it");
        create_dir(&plugins_lib_path).with_context(|| "Failed to create plugins directory")?;
    }
    debug!("Main jar path {}", main_jar_path.display());
    debug!("Plugins path {}", plugins_lib_path.display());
    let plugin_jar_paths = get_jar_list(plugins_lib_path)?;
    let mut jar_paths = Vec::new();
    jar_paths.push(main_jar_path);
    jar_paths.extend(plugin_jar_paths);
    Ok(paths_to_strs(&jar_paths)?.join(":"))
}
