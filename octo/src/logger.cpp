#include "../include/logger.hpp"
#include "../include/utils.hpp"

#include <cstdarg>

namespace octo
{
    void _print_current_time()
    {
        time_t ct = time(NULL);
        struct tm *lt = localtime(&ct);

        fprintf(
            stdout,
            "%d-%02d-%02d %02d:%02d:%02d ",
            1900 + lt->tm_year,
            lt->tm_mon,
            lt->tm_mday,
            lt->tm_hour,
            lt->tm_min,
            lt->tm_sec);
    }

    void _print_formated_va_list(FILE *const stream, const char *fmt, va_list argp)
    {
#ifndef OCTO_LOGGER_NO_SHOW_TIMESTAMP
        _print_current_time();
#endif
        vfprintf(stream, fmt, argp);
    }

    void log_debug(const char *fmt, ...)
    {
#ifndef __PRODUCTION__
        fprintf(stdout, "\n%s", OCTO_LOGGER_DEBUG);
        va_list argp;
        va_start(argp, fmt);
        _print_formated_va_list(stdout, fmt, argp);
        va_end(argp);
        fprintf(stdout, OCTO_LOGGER_RESET);
#endif
    }

    void log_info(const char *fmt, ...)
    {
        fprintf(stdout, "\n%s", OCTO_LOGGER_INFO);
        va_list argp;
        va_start(argp, fmt);
        _print_formated_va_list(stdout, fmt, argp);
        va_end(argp);
        fprintf(stdout, OCTO_LOGGER_RESET);
    }

    void log_system(const char *fmt, ...)
    {
        fprintf(stdout, "\n%s", OCTO_LOGGER_SYSTEM);
        va_list argp;
        va_start(argp, fmt);
        _print_formated_va_list(stdout, fmt, argp);
        va_end(argp);
        fprintf(stdout, OCTO_LOGGER_RESET);
    }

    void log_warn(const char *fmt, ...)
    {
        fprintf(stdout, "\n%s", OCTO_LOGGER_WARN);
        va_list argp;
        va_start(argp, fmt);
        _print_formated_va_list(stdout, fmt, argp);
        va_end(argp);
        fprintf(stdout, OCTO_LOGGER_RESET);
    }

    void log_error(const char *fmt, ...)
    {
        fprintf(stderr, "\n%s", OCTO_LOGGER_ERROR);
        va_list argp;
        va_start(argp, fmt);
        _print_formated_va_list(stderr, fmt, argp);
        va_end(argp);
        fprintf(stderr, OCTO_LOGGER_RESET);
    }

    void error_callback(int error, const char *desc)
    {
        log_error("ERROR (%d): %s", error, desc);
    }
}