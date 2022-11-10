#pragma once

#include <sstream>
#include <iostream>

#if defined WIN32 || defined __MINGW32__
#include <windows.h>
static bool win_console_initialized = false;
#endif

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define WHITE "\033[1m"
#define GREY "\033[2m"
#define CYAN "\033[36m"
#define RESET "\033[0;0m"

std::string GetNowTime();

template<typename ... Args>
void LogMessage(const char* level_color, const char* level_string, const Args&... args) {

#if defined WIN32 || defined __MINGW32__
    if (!win_console_initialized) {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

        if (handle != INVALID_HANDLE_VALUE) {
            DWORD mode = 0;
            if (GetConsoleMode(handle, &mode)) {
                mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(handle, mode);
            }
        }

        win_console_initialized = true;
    }
#endif

    std::stringstream ss;

    ss << CYAN << GetNowTime() << RESET;
    ss << ' ' << level_color << level_string;

    ((ss << ' ' << args), ...);

    ss << RESET;

    std::cout << ss.str() << std::endl;
}

template<typename ... Args>
void LogError(const Args&... args) {
    LogMessage(RED, "ERROR", args...);
}

template<typename ... Args>
void LogWarn(const Args&... args) {
    LogMessage(YELLOW, " WARN", args...);
}

template<typename ... Args>
void LogInfo(const Args&... args) {
    LogMessage(WHITE, " INFO", args...);
}

template<typename ... Args>
void LogDebug(const Args&... args) {
#ifndef NDEBUG
    LogMessage(GREY, "DEBUG", args...);
#endif
}