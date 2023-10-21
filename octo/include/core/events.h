#pragma once

#include "defines.h"

typedef enum event_type
{
    EVENT_TYPE_KEY_PRESSED,
    EVENT_TYPE_KEY_RELEASED,
    EVENT_TYPE_BUTTON_PRESSED,
    EVENT_TYPE_BUTTON_RELEASED,
    EVENT_TYPE_MOUSE_MOVED,
    EVENT_TYPE_MOUSE_WHEEL,
    EVENT_TYPE_WINDOW_RESIZED,

    EVENT_TYPE_LAST
} event_type;

typedef struct event_context
{
    union
    {
        i64 i64[2];
        u64 u64[2];
        f64 f64[2];

        i32 i32[4];
        u32 u32[4];
        f32 f32[4];

        i16 i16[8];
        u16 u16[8];

        i8 i8[16];
        u8 u8[16];

        char c[16];
    } data;
} event_context;

typedef void (*on_event_handler)(u16 code, void *sender, void *listener_inst, event_context data);

OCTO_API void on_event(event_type type, on_event_handler handler);
