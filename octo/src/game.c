#include "../include/core/game.h"
#include "../include/core/logger.h"

#include <GLFW/glfw3.h>
#include <stdlib.h>

typedef struct _state
{
    GLFWwindow *window;
} _state;

static _state state;

GameContext *create_game_context(const char *title)
{
    GameContext *ctx = calloc(1, sizeof(GameContext));
    ctx->title = title;
    ctx->version = OCTO_VERSION;
    ctx->width = GAME_DEFAULT_WIDTH;
    ctx->height = GAME_DEFAULT_HEIGHT;
    ctx->fullscreen = GAME_DEFAULT_FULLSCREEN;
    ctx->pressedThreshold = GAME_DEFAULT_PRESSED_THRESHOLD;

    return ctx;
}

void _keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    LOG_DEBUG("_keyCallback: %c was %s", key, action == GLFW_PRESS ? "pressed" : "released");
}

void _mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    LOG_DEBUG("_mouseButtonCallback: %c was %s", button, action == GLFW_PRESS ? "pressed" : "released");
}

void _mouseWheelCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    LOG_DEBUG("_mouseWheelCallback: x: %i, y: %i", xoffset, yoffset);
}

void _mousePositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    LOG_DEBUG("_mousePositionCallback: x: %i, y: %i", xpos, ypos);
}

void _resizeCallback(GLFWwindow *window, int width, int height)
{
    LOG_DEBUG("_resizeCallback: width: %i, height: %i", width, height);
}

bool game_init(GameContext *ctx)
{
    LOG_DEBUG("GameContext:");
    LOG_DEBUG("- title: %s", ctx->title);
    LOG_DEBUG("- version: %s", ctx->version);
    LOG_DEBUG("- width: %i", ctx->width);
    LOG_DEBUG("- height: %i", ctx->height);
    LOG_DEBUG("- fullscreen: %s", ctx->fullscreen ? "true" : "false");
    LOG_DEBUG("- pressedThreshold: %i", ctx->pressedThreshold);

    LOG_INFO("game_init: initializing window...");

    if (glfwInit() == GLFW_FALSE)
    {
        LOG_FATAL("game_init: failed to initialize window");
        return false;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    state.window = glfwCreateWindow(
        ctx->width,
        ctx->height,
        ctx->title,
        NULL, NULL);

    if (!state.window)
    {
        LOG_FATAL("game_init: failed to create window");
        return false;
    }

    glfwSetKeyCallback(state.window, &_keyCallback);
    glfwSetMouseButtonCallback(state.window, &_mouseButtonCallback);
    glfwSetScrollCallback(state.window, &_mouseWheelCallback);
    glfwSetCursorPosCallback(state.window, &_mousePositionCallback);
    glfwSetFramebufferSizeCallback(state.window, &_resizeCallback);

    return true;
}

void game_poll_events()
{
    glfwPollEvents();

    if (glfwWindowShouldClose(state.window))
    {
        LOG_DEBUG("game_poll_events: window should close");
    }
}

void game_shutdown(GameContext *ctx)
{
    glfwDestroyWindow(state.window);
    glfwTerminate();
    free(ctx);
}