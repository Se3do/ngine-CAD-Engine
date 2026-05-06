#pragma once

#include <ngine/logging/log_level.hpp>
#include <ngine/logging/logger.hpp>

#include <chrono>
#include <iostream>
#include <string_view>

namespace ngine {

class ConsoleSink final : public LogSink {
   public:
    void write(LogLevel level, std::string_view category, std::string_view message,
               const std::source_location& loc) override;
};

}  // namespace ngine
