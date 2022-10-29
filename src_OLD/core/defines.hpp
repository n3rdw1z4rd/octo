#pragma once

#define TO_STR(x) #x
#define STR(x) TO_STR(x)

#ifndef ENGINE_NAME
#define ENGINE_NAME "octo"
#endif

#ifndef VERSION_MAJOR
#define VERSION_MAJOR 0
#endif

#ifndef VERSION_MINOR
#define VERSION_MINOR 0
#endif

#ifndef VERSION_PATCH
#define VERSION_PATCH 0
#endif

#ifndef VERSION
#define VERSION STR(VERSION_MAJOR) "." STR(VERSION_MINOR) "." STR(VERSION_PATCH)
#endif

#ifndef ENGINE_NAME_AND_VERSION
#define ENGINE_NAME_AND_VERSION ENGINE_NAME " v" VERSION
#endif

/* GLFW */
#define GLFW_INCLUDE_VULKAN

/* GLM */
// #define GLM_FORCE_RADIANS
// #define GLM_FORCE_DEPTH_ZERO_TO_ONE
// #define GLM_ENABLE_EXPERIMENTAL

/* STB Image */
// #define STB_IMAGE_IMPLEMENTATION

/* Tiny Obj Loader */
// #define TINYOBJLOADER_IMPLEMENTATION
