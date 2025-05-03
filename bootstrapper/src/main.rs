#![windows_subsystem = "windows"]

pub mod config;
pub mod consts;
pub mod jni_utils;
pub mod logger;
pub mod utils;

use crate::config::Config;
use crate::consts::{LIBS_DIR, PLUGINS_DIR};
use crate::jni_utils::{build_args_array, set_thread_class_loader};
use crate::utils::{build_path, get_jar_list, open_console, paths_to_strs, reset_signal, show_err};

use anyhow::{Context, Result};
use jni::{
    objects::{JObject, JValueGen},
    InitArgsBuilder, JavaVM,
};
use log::{debug, error, info};
use std::{
    env::{current_dir, current_exe, set_current_dir},
    fs::{create_dir, File},
    io::Read,
    path::PathBuf,
};

const LAUNCHER_VERSION: i32 = 1;

#[cfg(debug_assertions)]
const APP_DIR: &str = "D:\\Ark-Pets\\desktop\\build\\jpackage\\ArkPets";
#[cfg(not(debug_assertions))]
const APP_DIR: &str = "";

fn main() {
    if cfg!(debug_assertions) {
        let _ = set_current_dir(APP_DIR);
    }
    if let Ok(()) = logger::init(log::LevelFilter::Debug) {
        if let Err(err) = launcher_main() {
            error!("{:?}", err);
            show_err(format!("{:?}", err));
        }
    } else {
        show_err("Failed to init logger".to_string());
    };
}

fn launcher_main() -> Result<()> {
    // 1. Init Launcher
    #[cfg(debug_assertions)]
    open_console()?;

    info!("ArkPets Bootstrapper V{}", LAUNCHER_VERSION);

    let current_work = current_dir().with_context(|| "Cannot determine working path.")?;
    let mut app_dir = if cfg!(debug_assertions) {
        PathBuf::from(APP_DIR)
    } else {
        current_exe().with_context(|| "Cannot determine application path.")?
    };
    if cfg!(not(debug_assertions)) {
        app_dir.pop();
    }
    info!("Current working dir is {}", current_work.display());
    info!("Launcher dir is {}", app_dir.display());

    // 2. Load Config
    let toml_path = app_dir.join("launch.toml");
    debug!("Loading config {}", toml_path.display());
    let mut toml_reader = File::open(&toml_path)
        .with_context(|| format!("Cannot open config {}", toml_path.display()))?;
    let mut toml_content: String = String::new();
    toml_reader
        .read_to_string(&mut toml_content)
        .with_context(|| "Cannot read config")?;
    let config: Config =
        toml::from_str(&toml_content).with_context(|| "Cannot parse config toml")?;

    // 3. Prepare environment
    // 3.1. Platform
    if cfg!(target_os = "linux") {
        debug!("Reset single handlers");
        reset_signal().with_context(|| "Cannot reset single handlers")?;
    }
    if cfg!(target_os = "windows") && !cfg!(debug_assertions) && config.launcher.console {
        open_console().with_context(|| "Failed to open debug console")?;
    }
    if config.runtime.use_user_data {
        let user_data =
            dirs::data_local_dir().with_context(|| "Cannot determine user data path")?;
        if !user_data.exists() {
            create_dir(&user_data).with_context(|| "Cannot create user data path")?;
        }
        info!("Setting working dir to {}", &user_data.display());
        set_current_dir(user_data).with_context(|| "Failed to set working dir")?;
    }
    // 3.2. Dirs and Jars
    info!("Searching for the main jar");
    let main_jar_path = build_path(
        &app_dir,
        &[LIBS_DIR, &*format!("desktop-{}.jar", config.arkpets.ver)],
    );
    let plugins_lib_path = build_path(&app_dir, &[PLUGINS_DIR]);
    debug!("Main jar path {}", main_jar_path.display());
    debug!("Plugins path {}", plugins_lib_path.display());
    let plugin_jar_paths = get_jar_list(plugins_lib_path)?;
    let mut jar_paths = Vec::new();
    jar_paths.push(main_jar_path);
    jar_paths.extend(plugin_jar_paths);
    let jars = paths_to_strs(&jar_paths)?.join(":");
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
    let jvm_arg = jvm_arg_builder
        .build()
        .with_context(|| "Failed to build JVM args.")?;
    let jvm = JavaVM::new(jvm_arg).with_context(|| "Failed to launch JVM.")?;

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
