// #pragma once

// #include "defines.h"

// typedef enum event_type
// {
//     EVENT_TYPE_SHUTDOWN,
//     EVENT_TYPE_KEY_PRESSED,
//     EVENT_TYPE_KEY_RELEASED,
//     EVENT_TYPE_BUTTON_PRESSED,
//     EVENT_TYPE_BUTTON_RELEASED,
//     EVENT_TYPE_MOUSE_MOVED,
//     EVENT_TYPE_MOUSE_WHEEL,
//     EVENT_TYPE_RESIZED,

//     EVENT_TYPE_LAST
// } event_type;

// typedef struct game_context
// {
//     const char *title;
//     const char *version;
//     u32 width;
//     u32 height;
//     bool fullscreen;

//     bool is_running;
//     bool is_paused;

//     u8 pressedThreshold;

//     // void (*event_handler)(struct game_context *context, event_type event_type, void *data);

//     // void (*keyboard_handler)(u32 key, u32 mods, bool pressed);
//     // void (*mouse_button_handler)(u32 button, u32 mods, bool pressed);
//     // void (*mouse_wheel_handler)(u32 x_offset, u32 y_offset);
//     // void (*mouse_move_handler)(u32 x_pos, u32 y_pos);
//     // void (*window_resize_handler)(u32 width, u32 height);
// } game_context;

// game_context *create_game_context(const char *);
// OCTO_API bool game_init(game_context *);
// OCTO_API void game_poll_events();
// OCTO_API void game_shutdown(game_context *);
