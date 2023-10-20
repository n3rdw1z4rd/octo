#pragma once

#include "defines.h"

#define GAME_DEFAULT_WIDTH 1280
#define GAME_DEFAULT_HEIGHT 720
#define GAME_DEFAULT_FULLSCREEN 0
#define GAME_DEFAULT_PRESSED_THRESHOLD 200

typedef struct GameContext
{
    const char *title;
    const char *version;
    u32 width;
    u32 height;
    bool fullscreen;
    u8 pressedThreshold;
} GameContext;

OCTO_API GameContext *create_game_context(const char *);
OCTO_API bool game_init(GameContext *);
OCTO_API void game_poll_events();
OCTO_API void game_shutdown(GameContext *);
