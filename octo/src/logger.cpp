#include "../include/core/logger.hpp"

// _get_now_time
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

// _log
#include <cstdarg>
#include <iostream>
#include <string>

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define WHITE "\033[1m"
#define GREY "\033[2m"

#define CYAN "\033[36m"
#define RESET "\033[0;0m"

#ifdef OCTO_PLATFORM_WINDOWS
#include <windows.h>
static bool win_console_initialized = false;
#endif

namespace octo
{
    const char *_colors[6] = {RED, RED, YELLOW, WHITE, GREY, GREY};

    const char *_levels[6] = {
        "[FATAL]",
        "[ERROR]",
        "[WARN] ",
        "[INFO] ",
        "[DEBUG]",
        "[TRACE]"};

    std::string
    _get_now_time()
    {
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        auto timer = std::chrono::system_clock::to_time_t(now);

        std::tm bt = *std::localtime(&timer);
        std::ostringstream oss;

        oss << std::put_time(&bt, "%Y-%m-%d %H:%M:%S");
        oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

        return oss.str();
    }

    // OCTO_API void _log(LogLevel level, const char *string_format, ...)
    // {
    //     const bool is_error = (level < LOG_LEVEL_WARN);

    // }

    template <typename... Args>
    void LogMessage(LogLevel log_level, const Args &...args)
    {
#ifdef OCTO_PLATFORM_WINDOWS
        if (!win_console_initialized)
        {
            HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

            if (handle != INVALID_HANDLE_VALUE)
            {
                DWORD mode = 0;
                if (GetConsoleMode(handle, &mode))
                {
                    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                    SetConsoleMode(handle, mode);
                }
            }

            win_console_initialized = true;
        }
#endif
        const bool is_error = (log_level < LOG_LEVEL_WARN);

        std::stringstream ss;

        ss << CYAN << _get_now_time() << RESET;
        ss << ' ' << _colors[log_level] << _levels[log_level];

        ((ss << ' ' << args), ...);

        ss << RESET;

        std::cout << ss.str() << std::endl;
    }
}