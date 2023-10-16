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

#if defined(PLATFORM_LINUX)
#if defined(WAYLAND_DISPLAY)
#define DISPLAY_WAYLAND
#elif defined(DISPLAY)
#define DISPLAY_X11
#endif
#endif