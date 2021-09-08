#pragma once

#include "defines.hpp"

#include <sstream>
#include <iostream>

const char* get_now_time();

template<typename ... Args>
void _log(const char* level_color, const char* level_string, const Args&... args) {
    std::stringstream ss;

    ss << "\033[36m" << get_now_time() << "\033[0;0m" << ' ';
    ss << level_color << level_string << ' ';

    ((ss << args), ...);

    ss << "\033[0;0m";

    std::cout << ss.str() << std::endl;
}

template<typename ... Args>
void log_error(const Args&... args) {
    _log("\033[31m", "ERROR", args...);
}

template<typename ... Args>
void log_warn(const Args&... args) {
    _log("\033[33m", " WARN", args...);
}

template<typename ... Args>
void log_info(const Args&... args) {
    _log("\033[1m", " INFO", args...);
}

template<typename ... Args>
void log_debug(const Args&... args) {
#ifndef NDEBUG
    _log("\033[2m", "DEBUG", args...);
#endif
}