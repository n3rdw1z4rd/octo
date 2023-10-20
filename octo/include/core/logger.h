#pragma once

#include "defines.h"

typedef enum LogLevel
{
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE
};

// OCTO_API void _log(LogLevel level, const char* string_format, ...);

template <typename... Args>
void LogMessage(LogLevel log_level, const Args &...args);

// template <typename... Args>
// void LogFatal(const Args &...args)
// {
//     LogMessage(LOG_LEVEL_FATAL, args...);
// }

// template <typename... Args>
// void LogError(const Args &...args)
// {
//     LogMessage(LOG_LEVEL_ERROR, args...);
// }

// template <typename... Args>
// void LogWarn(const Args &...args)
// {
//     LogMessage(LOG_LEVEL_WARN, args...);
// }

// template <typename... Args>
// void LogInfo(const Args &...args)
// {
//     LogMessage(LOG_LEVEL_INFO, args...);
// }

// template <typename... Args>
// void LogDebug(const Args &...args)
// {
//     LogMessage(LOG_LEVEL_DEBUG, args...);
// }

// template <typename... Args>
// void LogTrace(const Args &...args)
// {
//     LogMessage(LOG_LEVEL_TRACE, args...);
// }
