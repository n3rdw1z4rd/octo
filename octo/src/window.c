#include "../include/core/window.h"
#include "../include/core/logger.h"

#include <stdlib.h>

typedef struct internal_state
{
    GLFWwindow *handle;
    window *window;
    u64 mouse_position_x;
    u64 mouse_position_y;
    u32 key_down[OCTO_KEY_LAST];
    u32 button_down[OCTO_MOUSE_BUTTON_LAST];
} internal_state;

static bool initialized = false;
static internal_state state;

void _keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key > OCTO_KEY_UNKNOWN)
    {
        state.key_down[key] = (action == GLFW_PRESS ? true : false);
    }
}

void _mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button > OCTO_MOUSE_BUTTON_UNKNOWN)
    {
        state.button_down[button] = (action == GLFW_PRESS ? true : false);
    }
}

void _mouseWheelCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    LOG_DEBUG("_mouseWheelCallback: x: %i, y: %i", xoffset, yoffset);
}

void _mousePositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    state.mouse_position_x = xpos;
    state.mouse_position_y = ypos;
}

void _resizeCallback(GLFWwindow *window, int width, int height)
{
    LOG_DEBUG("_resizeCallback (unhandled):");
    LOG_DEBUG("- width: %i", width);
    LOG_DEBUG("- height: %i", height);
}

window *create_window(const char *title, u32 width, u32 height)
{
    if (!initialized)
    {
        LOG_DEBUG("create_window: initializing...");
        LOG_DEBUG("- title: %s", title);
        LOG_DEBUG("- width: %i", width);
        LOG_DEBUG("- height: %i", height);

        if (glfwInit() == GLFW_FALSE)
        {
            LOG_FATAL("create_window: failed to initialize GLFW");
            return NULL;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        state.handle = glfwCreateWindow(width, height, title, NULL, NULL);

        if (!state.handle)
        {
            LOG_FATAL("create_window: failed to create window");
            return NULL;
        }

        glfwSetKeyCallback(state.handle, &_keyCallback);
        glfwSetMouseButtonCallback(state.handle, &_mouseButtonCallback);
        glfwSetScrollCallback(state.handle, &_mouseWheelCallback);
        glfwSetCursorPosCallback(state.handle, &_mousePositionCallback);
        glfwSetFramebufferSizeCallback(state.handle, &_resizeCallback);

        state.window = malloc(sizeof(window));
        state.window->title = title;
        state.window->width = width;
        state.window->height = height;
        state.window->should_close = false;
        // window->handle = state.handle;
        // window->mouse_position.x = 0;
        // window->mouse_position.y = 0;

        initialized = true;

        return state.window;
    }
    else
    {
        LOG_WARN("create_window: already initialized");
        return state.window;
    }
}

void destroy_window()
{
    LOG_DEBUG("destroy_window: destroying window");
    glfwDestroyWindow(state.handle);
    glfwTerminate();
    free(state.window);
}

void poll_window_events()
{
    if (initialized)
    {
        glfwPollEvents();

        if (glfwWindowShouldClose(state.handle) != 0)
        {
            state.window->should_close = true;
        }
    }
}

mouse_position get_mouse_postion()
{
    mouse_position mpos;
    mpos.x = state.mouse_position_x;
    mpos.y = state.mouse_position_y;
    return mpos;
}

bool is_key_down(u32 key_code)
{
    if (key_code < OCTO_KEY_LAST)
    {
        return state.key_down[key_code];
    }
    else
    {
        return false;
    }
}
