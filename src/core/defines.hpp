#pragma once

#ifndef PROJECT_NAME
#define PROJECT_NAME "octo"
#endif

#ifndef DEFAULT_CONFIG_PATH
#define DEFAULT_CONFIG_PATH "octo.json"
#endif

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define ARRAYSIZE(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR))))

// #include "config.hpp"
