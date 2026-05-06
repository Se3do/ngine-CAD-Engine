#pragma once

#include <ngine/logging/log_level.hpp>

#include <memory>
#include <mutex>
#include <source_location>
#include <string>
#include <string_view>
#include <vector>

namespace ngine {

class LogSink {
public:
    virtual ~LogSink() = default;
    virtual void write(LogLevel level, std::string_view category, std::string_view message,
                       const std::source_location& loc) = 0;
};

class Logger {
public:
    static Logger& instance();

    void add_sink(std::shared_ptr<LogSink> sink);
    void set_level(LogLevel level);
    [[nodiscard]] LogLevel level() const noexcept;

    void log(LogLevel level, std::string_view category, std::string_view message,
             const std::source_location& loc = std::source_location::current());

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger() = default;

    std::vector<std::shared_ptr<LogSink>> sinks_;
    LogLevel min_level_{LogLevel::Info};
    mutable std::mutex mutex_;
};

}  // namespace ngine
