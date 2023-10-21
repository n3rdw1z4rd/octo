// #include "../include/core/game.h"
// #include "../include/core/logger.h"

// #include <GLFW/glfw3.h>
// #include <stdlib.h>

// #define GAME_DEFAULT_WIDTH 1280
// #define GAME_DEFAULT_HEIGHT 720
// #define GAME_DEFAULT_FULLSCREEN 0
// #define GAME_DEFAULT_PRESSED_THRESHOLD 200

// typedef struct _state
// {
//     GLFWwindow *window;
//     game_context *context;

    
// } _state;

// static _state state;

// game_context *create_game_context(const char *title)
// {
//     game_context *ctx = calloc(1, sizeof(game_context));
//     ctx->title = title;
//     ctx->version = OCTO_VERSION;
//     ctx->width = GAME_DEFAULT_WIDTH;
//     ctx->height = GAME_DEFAULT_HEIGHT;
//     ctx->fullscreen = GAME_DEFAULT_FULLSCREEN;
//     ctx->pressedThreshold = GAME_DEFAULT_PRESSED_THRESHOLD;
//     ctx->is_running = false;
//     ctx->is_paused = false;

//     return ctx;
// }

// void emit_event(event_type event_type, void *data)
// {
//     // if (state.handler != NULL)
//     // {
//     //     state.handler(state.context, event_type, NULL);
//     // }

//     if (state.context->event_handler != NULL)
//     {
//         state.context->event_handler(state.context, event_type, NULL);
//     }
// }

// void _keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
// {
//     // int *data = (int *)malloc(sizeof(int));
//     // *data = key;

//     emit_event(action == GLFW_PRESS ? EVENT_TYPE_KEY_PRESSED : EVENT_TYPE_KEY_RELEASED, &key);//data);
// }

// void _mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
// {
//     LOG_DEBUG("_mouseButtonCallback: %c was %s", button, action == GLFW_PRESS ? "pressed" : "released");
// }

// void _mouseWheelCallback(GLFWwindow *window, double xoffset, double yoffset)
// {
//     LOG_DEBUG("_mouseWheelCallback: x: %i, y: %i", xoffset, yoffset);
// }

// void _mousePositionCallback(GLFWwindow *window, double xpos, double ypos)
// {
//     // LOG_DEBUG("_mousePositionCallback: x: %i, y: %i", xpos, ypos);
// }

// void _resizeCallback(GLFWwindow *window, int width, int height)
// {
//     LOG_DEBUG("_resizeCallback: width: %i, height: %i", width, height);
// }

// bool game_init(game_context *ctx)//, event_handler handler)
// {
//     LOG_DEBUG("game_context:");
//     LOG_DEBUG("- title: %s", ctx->title);
//     LOG_DEBUG("- version: %s", ctx->version);
//     LOG_DEBUG("- width: %i", ctx->width);
//     LOG_DEBUG("- height: %i", ctx->height);
//     LOG_DEBUG("- fullscreen: %s", ctx->fullscreen ? "true" : "false");
//     LOG_DEBUG("- pressedThreshold: %i", ctx->pressedThreshold);

//     LOG_INFO("game_init: initializing window...");

//     if (glfwInit() == GLFW_FALSE)
//     {
//         LOG_FATAL("game_init: failed to initialize window");
//         return false;
//     }

//     glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

//     state.window = glfwCreateWindow(
//         ctx->width,
//         ctx->height,
//         ctx->title,
//         NULL, NULL);

//     if (!state.window)
//     {
//         LOG_FATAL("game_init: failed to create window");
//         return false;
//     }

//     glfwSetKeyCallback(state.window, &_keyCallback);
//     glfwSetMouseButtonCallback(state.window, &_mouseButtonCallback);
//     glfwSetScrollCallback(state.window, &_mouseWheelCallback);
//     glfwSetCursorPosCallback(state.window, &_mousePositionCallback);
//     glfwSetFramebufferSizeCallback(state.window, &_resizeCallback);

//     ctx->is_running = true;
//     state.context = ctx;
//     // state.handler = handler;

//     return true;
// }

// void game_poll_events()
// {
//     glfwPollEvents();

//     if (glfwWindowShouldClose(state.window))
//     {
//         emit_event(EVENT_TYPE_SHUTDOWN, NULL);
//     }
// }

// void game_shutdown(game_context *ctx)
// {
//     glfwDestroyWindow(state.window);
//     glfwTerminate();
//     free(ctx);
// }