use anyhow::Result;
use jni::{
    objects::{JClass, JObject, JObjectArray, JValueGen},
    strings::JNIString,
    AttachGuard,
};
use std::path::PathBuf;

pub fn set_thread_class_loader(env: &mut AttachGuard, main_class: &JClass) -> Result<()> {
    // We must set the context ClassLoader for JavaFX FXML in JNI.
    let thread_class = env.find_class("java/lang/Thread")?;
    let current_jvm_thread = env
        .call_static_method(thread_class, "currentThread", "()Ljava/lang/Thread;", &[])?
        .l()?;
    let current_class_loader = env
        .call_method(
            main_class,
            "getClassLoader",
            "()Ljava/lang/ClassLoader;",
            &[],
        )?
        .l()?;
    env.call_method(
        current_jvm_thread,
        "setContextClassLoader",
        "(Ljava/lang/ClassLoader;)V",
        &[JValueGen::Object(&current_class_loader)],
    )?
    .v()?;
    Ok(())
}

pub fn build_args_array<'a, T>(args: T, env: &mut AttachGuard<'a>) -> Result<JObjectArray<'a>>
where
    T: IntoIterator,
    T::Item: Into<JNIString>,
{
    let iter = args.into_iter();
    let mut j_str = Vec::new();
    let mut count = 0;
    let arr_string_class = env.find_class("java/lang/String")?;
    for arg in iter {
        let str = env.new_string(arg)?;
        j_str.push(str);
        count += 1;
    }
    let j_args = env.new_object_array(count, arr_string_class, JObject::null())?;
    for (i, arg) in j_str.iter().enumerate() {
        env.set_object_array_element(&j_args, i as i32, arg)?;
    }
    Ok(j_args)
}

pub fn find_libjvm(app: &PathBuf, local_jvm: bool) -> PathBuf {
    if local_jvm {
        [
            java_locator::locate_jvm_dyn_library()
                .expect("Cannot find local java")
                .as_str(),
            java_locator::get_jvm_dyn_lib_file_name(),
        ]
        .iter()
        .collect::<PathBuf>()
    } else {
        cfg_if::cfg_if! {
            if #[cfg(target_os = "windows")] {
                app.join("runtime\\bin\\server\\jvm.dll")
            } else if #[cfg(target_os = "linux")] {
                app.join("runtime/bin/server/libjvm.so")
            } else if #[cfg(target_os = "macos")] {
                app.join("runtime/Contents/Home/lib/server/libjvm.dylib")
            }
        }
    }
}
