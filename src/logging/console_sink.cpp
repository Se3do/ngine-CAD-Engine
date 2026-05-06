#include <ngine/logging/console_sink.hpp>

#include <chrono>
#include <format>
#include <iostream>

namespace ngine {

void ConsoleSink::write(LogLevel level, std::string_view category, std::string_view message,
                        const std::source_location& loc) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::floor<std::chrono::milliseconds>(now);

    std::cerr << std::format("[{:%H:%M:%S}] [{}] [{}] {} ({}:{})\n", time, to_string(level),
                             category, message, loc.file_name(), loc.line());
}

}  // namespace ngine
