#[cfg(target_os = "windows")]
extern crate windows_exe_info;
fn main() {
    #[cfg(target_os = "windows")]
    {
        windows_exe_info::icon::icon_ico("./icon.ico");
        windows_exe_info::versioninfo::link_cargo_env()
    }
}
