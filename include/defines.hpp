#pragma once

#include "version.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__MINGW32__)
#include <windows.h>
static bool win_console_initialized = false;
#endif

// #define GLFW_CLIENT_NO_API
#define GLFW_INCLUDE_VULKAN

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL

#define STB_IMAGE_IMPLEMENTATION

#define TINYOBJLOADER_IMPLEMENTATION
