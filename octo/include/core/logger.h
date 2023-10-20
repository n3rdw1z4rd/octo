#pragma once

#include "defines.h"

#define OCTO_MAX_LOG_LENGTH 1024

typedef enum LogLevel
{
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE
} LogLevel;

OCTO_API void __log_message(LogLevel log_level, const char *msg_format, ...);

#ifndef LOG_FATAL
#define LOG_FATAL(msg, ...) __log_message(LOG_LEVEL_FATAL, msg, ##__VA_ARGS__)
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(msg, ...) __log_message(LOG_LEVEL_ERROR, msg, ##__VA_ARGS__)
#endif

#ifndef LOG_WARN
#define LOG_WARN(msg, ...) __log_message(LOG_LEVEL_WARN, msg, ##__VA_ARGS__)
#endif

#ifndef LOG_INFO
#define LOG_INFO(msg, ...) __log_message(LOG_LEVEL_INFO, msg, ##__VA_ARGS__)
#endif

#ifndef LOG_DEBUG
#define LOG_DEBUG(msg, ...) __log_message(LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__)
#endif

#ifndef LOG_TRACE
#define LOG_TRACE(msg, ...) __log_message(LOG_LEVEL_TRACE, msg, ##__VA_ARGS__)
#endif
