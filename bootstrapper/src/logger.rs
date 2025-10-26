use log::{LevelFilter, Metadata, Record, SetLoggerError};

pub fn init(filter: LevelFilter) -> Result<(), SetLoggerError> {
    let logger = Logger {};
    let result = log::set_boxed_logger(Box::new(logger));
    if result.is_ok() {
        log::set_max_level(filter);
    }
    result
}

struct Logger {}

impl log::Log for Logger {
    fn enabled(&self, _metadata: &Metadata<'_>) -> bool {
        true
    }

    fn log(&self, record: &Record<'_>) {
        println!("[{}] Bootstrapper: {}", record.level(), record.args());
    }

    fn flush(&self) {}
}
