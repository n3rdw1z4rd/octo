#pragma once

#include "defines.h"

#include <GLFW/glfw3.h>

typedef struct window
{
    const char *title;
    f64 width;
    f64 height;
    bool fullscreen;
    bool should_close;
} window;

typedef struct mouse_position
{
    u64 x;
    u64 y;
} mouse_position;

OCTO_API window *create_window(const char *title, u32 width, u32 height);
OCTO_API void destroy_window();
OCTO_API void poll_window_events();
OCTO_API mouse_position get_mouse_postion();
OCTO_API bool is_key_down(u32 key_code);
// OCTO_API bool was_key_pressed(u32 key_code);
