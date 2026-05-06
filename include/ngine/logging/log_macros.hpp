#pragma once

#include <ngine/logging/logger.hpp>

#define NGINE_LOG(level, cat, msg) \
    ::ngine::Logger::instance().log(level, cat, msg)

#define NGINE_LOG_TRACE(cat, msg) NGINE_LOG(::ngine::LogLevel::Trace, cat, msg)
#define NGINE_LOG_DEBUG(cat, msg) NGINE_LOG(::ngine::LogLevel::Debug, cat, msg)
#define NGINE_LOG_INFO(cat, msg) NGINE_LOG(::ngine::LogLevel::Info, cat, msg)
#define NGINE_LOG_WARN(cat, msg) NGINE_LOG(::ngine::LogLevel::Warn, cat, msg)
#define NGINE_LOG_ERROR(cat, msg) NGINE_LOG(::ngine::LogLevel::Error, cat, msg)
#define NGINE_LOG_FATAL(cat, msg) NGINE_LOG(::ngine::LogLevel::Fatal, cat, msg)
