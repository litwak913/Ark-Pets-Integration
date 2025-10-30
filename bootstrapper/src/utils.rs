use std::path::PathBuf;

use anyhow::{anyhow, Context, Result};
use native_dialog::{DialogBuilder, MessageLevel};

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
