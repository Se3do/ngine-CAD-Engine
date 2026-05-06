#include <ngine/logging/file_sink.hpp>

#include <chrono>
#include <format>
#include <stdexcept>

namespace ngine {

FileSink::FileSink(const std::filesystem::path& path) : file_(path, std::ios::app) {
    if (!file_.is_open()) {
        throw std::runtime_error("Failed to open log file: " + path.string());
    }
}

FileSink::~FileSink() = default;

void FileSink::write(LogLevel level, std::string_view category, std::string_view message,
                     const std::source_location& loc) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::floor<std::chrono::milliseconds>(now);

    file_ << std::format("[{:%Y-%m-%d %H:%M:%S}] [{}] [{}] {} ({}:{})\n", time, to_string(level),
                         category, message, loc.file_name(), loc.line());
    file_.flush();
}

}  // namespace ngine
