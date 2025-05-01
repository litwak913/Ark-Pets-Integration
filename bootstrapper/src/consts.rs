pub const LIBS_DIR: &str = "libs";
pub const PLUGINS_DIR: &str = "plugins";
#[cfg(target_os = "windows")]
pub const PLATFORM_LIB: &str = "windows-x64";
#[cfg(target_os = "linux")]
pub const PLATFORM_LIB: &str = "linux-x64";
#[cfg(all(target_os = "macos", target_arch = "x86_64"))]
pub const PLATFORM_LIB: &str = "macos-x64";
#[cfg(all(target_os = "macos", target_arch = "aarch64"))]
pub const PLATFORM_LIB: &str = "macos-aarch64";
