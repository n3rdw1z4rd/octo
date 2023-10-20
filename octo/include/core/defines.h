#pragma once

#include "version.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef float f32;
typedef double f64;

typedef unsigned char bool;

#if !NDEBUG
#define OCTO_DEBUG
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define OCTO_PLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64bit is required on Winblows!"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
#define OCTO_PLATFORM_LINUX 1
#if defined(__ANDROID__)
#define OCTO_PLATFORM_ANDROID 1
#endif
#elif defined(__unix__)
#define OCTO_PLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
#define OCTO_PLATFORM_POXIS 1
#elif __APPLE__
#define OCTO_PLATFORM_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
#define OCTO_PLATFORM_IOS 1
#define OCTO_PLATFORM_IOS_SIMULATOR 1
#elif TARGET_OS_IPHONE
#define OCTO_PLATFORM_IOS 1
#elif TARGET_OS_MAC
#define OCTO_PLATFORM_MACOS 1
#else
#error "Unknown Apple platform"
#endif
#else
#error "Unknown platform"
#endif

#ifdef OCTO_EXPORT
#ifdef _MSC_VER
#define OCTO_API __declspec(dllexport)
#else
#define OCTO_API __attribute__((visibility("default")))
#endif
#else
#ifdef _MSC_VER
#define OCTO_API __declspec(dllimport)
#else
#define OCTO_API
#endif
#endif

#define OCTO_VULKAN_API_VERSION VK_API_VERSION_1_3

#define GLFW_CLIENT_NO_API
// #define GLFW_INCLUDE_VULKAN

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL

// #define STB_IMAGE_IMPLEMENTATION
// #define TINYOBJLOADER_IMPLEMENTATION
