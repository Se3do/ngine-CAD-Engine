#pragma once

#include <ngine/logging/log_level.hpp>
#include <ngine/logging/logger.hpp>

#include <filesystem>
#include <fstream>
#include <string_view>

namespace ngine {

class FileSink final : public LogSink {
   public:
    explicit FileSink(const std::filesystem::path& path);
    ~FileSink() override;

    void write(LogLevel level, std::string_view category, std::string_view message,
               const std::source_location& loc) override;

   private:
    std::ofstream file_;
};

}  // namespace ngine
