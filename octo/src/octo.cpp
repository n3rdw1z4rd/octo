#include "../include/octo.hpp"

namespace octo
{
    Context *create_default_context()
    {
        log_debug("create_default_context");

        Context *context = new Context;

        context->windowTitle = OCTO_WINDOW_DEFAULT_TITLE;
        context->windowWidth = OCTO_WINDOW_DEFAULT_WIDTH;
        context->windowHeight = OCTO_WINDOW_DEFAULT_HEIGHT;
        context->fullscreen = OCTO_WINDOW_DEFAULT_FULLSCREEN;

        return context;
    }

    bool init(Context *context)
    {
        log_debug("initializing %s", OCTO_WINDOW_DEFAULT_TITLE);

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        context->windowHandle = glfwCreateWindow(context->windowWidth, context->windowHeight, context->windowTitle.c_str(), nullptr, nullptr);

        if (!context->windowHandle)
        {
            log_error("octo::init: failed to create native application window");
            return false;
        }

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        log_debug("supported extensions: %d", extensionCount);

        return true;
    }

    void shutdown(Context *context)
    {
        log_debug("shutdown");

        glfwDestroyWindow(context->windowHandle);
        glfwTerminate();

        log_debug("bye!\n\n");
    }

    void main_loop(Context *context)
    {
        log_debug("main_loop");

        while (!glfwWindowShouldClose(context->windowHandle))
        {
            glfwPollEvents();
        }
    }
}