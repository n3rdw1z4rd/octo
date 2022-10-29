#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>

#include "defines.hpp"

void log_debug(const char *, ...);
void log_info(const char *, ...);
void log_warn(const char *, ...);
void log_error(const char *, ...);
void error_callback(int, const char *);
