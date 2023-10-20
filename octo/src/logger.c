#include "../include/core/logger.h"
#include "../include/core/time.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

#define CYAN "\033[36m"
#define RESET "\033[0;0m"

#ifdef OCTO_PLATFORM_WINDOWS
#include <windows.h>
static bool win_console_initialized = false;
#endif

const char *_color_strings[6] = {
    "\033[1;41m",  // fatal, white on red, bold
    "\033[1;31m",  // error, red on black, bold
    "\033[1;33m",  // warn, yellow on black, bold
    "\033[1;97m",  // info, white on black, bold
    "\033[1;90m",  // debug, light gray on black, bold
    "\033[0;90m"}; // trace, dark grey on black

const char *_level_strings[6] = {
    "[FTL]",
    "[ERR]",
    "[WRN]",
    "[INF]",
    "[DBG]",
    "[TRC]"};

void __log_message(LogLevel log_level, const char *msg_format, ...)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char args[OCTO_MAX_LOG_LENGTH];
    memset(args, 0, sizeof(args));

    __builtin_va_list ptr;
    va_start(ptr, msg_format);
    vsnprintf(args, OCTO_MAX_LOG_LENGTH, msg_format, ptr);
    va_end(ptr);

    printf(
        "%s%d-%02d-%02d %02d:%02d:%02d%s %s%s %s%s\n",
        CYAN,
        tm.tm_year + 1900,
        tm.tm_mon + 1,
        tm.tm_mday,
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec,
        RESET,
        _color_strings[log_level],
        _level_strings[log_level],
        args,
        RESET);
}
