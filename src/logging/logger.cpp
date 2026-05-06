#include <ngine/logging/logger.hpp>

namespace ngine {

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

void Logger::add_sink(std::shared_ptr<LogSink> sink) {
    std::lock_guard lock(mutex_);
    sinks_.push_back(std::move(sink));
}

void Logger::set_level(LogLevel level) {
    std::lock_guard lock(mutex_);
    min_level_ = level;
}

LogLevel Logger::level() const noexcept {
    return min_level_;
}

void Logger::log(LogLevel level, std::string_view category, std::string_view message,
                 const std::source_location& loc) {
    if (level < min_level_) {
        return;
    }

    std::lock_guard lock(mutex_);
    for (auto& sink : sinks_) {
        sink->write(level, category, message, loc);
    }
}

}  // namespace ngine
