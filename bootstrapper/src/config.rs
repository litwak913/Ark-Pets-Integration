use serde::Deserialize;

#[derive(Deserialize, Debug)]
pub struct Config {
    pub arkpets: ArkPetsConfig,
    pub runtime: RuntimeConfig,
    pub launcher: LauncherConfig,
}
#[derive(Deserialize, Debug)]
pub struct ArkPetsConfig {
    pub args: Vec<String>,
    pub main: String,
}
#[derive(Deserialize, Debug)]
pub struct RuntimeConfig {
    pub jvm_args: Vec<String>,
    pub use_user_data: bool,
}
#[derive(Deserialize, Debug)]
pub struct LauncherConfig {
    pub console: bool,
    pub log_level: usize,
}
