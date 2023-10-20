#include "../include/core/logger.h"
#include "../include/core/time.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define CYAN "\033[36m"
#define RESET "\033[0;0m"

#ifdef OCTO_PLATFORM_WINDOWS
#include <windows.h>
static bool win_console_initialized = false;
#endif

const char *_color_strings[6] = {
    "\033[1;41m",
    "\033[1;31m",
    "\033[1;33m",
    "\033[1;32m",
    "\033[1;34m",
    "\033[1;30m"};

const char *_level_strings[6] = {
    "[FATAL]",
    "[ERROR]",
    "[WARN] ",
    "[INFO] ",
    "[DEBUG]",
    "[TRACE]"};

void __log_message(LogLevel log_level, const char *msg_format, ...)
{
    bool is_error = (log_level < LOG_LEVEL_WARN);

    char args[OCTO_MAX_LOG_LENGTH];
    memset(args, 0, sizeof(args));

    __builtin_va_list ptr;
    va_start(ptr, msg_format);
    vsnprintf(args, OCTO_MAX_LOG_LENGTH, msg_format, ptr);
    va_end(ptr);

    printf(
        "%s%s%s %s%s %s%s\n",
        CYAN,
        get_date_time_stamp(),
        RESET,
        _color_strings[log_level],
        _level_strings[log_level],
        args,
        RESET);
}
