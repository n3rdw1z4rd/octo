#pragma once

#include <cstdint>
#include <string>

typedef struct application_config {
    std::string window_title;
    uint32_t window_width;
    uint32_t window_height;
} application_config;
