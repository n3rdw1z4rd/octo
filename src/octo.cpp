#include "../include/octo.hpp"

bool octo_init()
{
#ifndef __PRODUCTION__
    log_warn("*** NON_PRODUCTION_BUILD ***");
#endif

#ifdef _WIN32
    log_debug("*** WINBLOWS ***");
#else
    log_debug("*** UNIX ***");
#endif

    log_debug("octo_init()");

    if (glfwInit() == GLFW_FALSE)
    {
        log_error("\t* failed to initialize glfw");
        return false;
    }

    // glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // glfwSetErrorCallback(error_callback);

    return true;
}

GLFWwindow *octo_create_window(unsigned int width, unsigned int height, const char *title, bool fullscreen)
{
    log_debug("octo_create_window()");

    GLFWwindow *handle = glfwCreateWindow(width, height, title, NULL, NULL);

    if (handle == NULL)
    {
        log_error("\t* failed to create window");
    }

    return handle;
}

GLFWwindow *octo_create_default_window()
{
    return octo_create_window(
        OCTO_WINDOW_DEFAULT_WIDTH,
        OCTO_WINDOW_DEFAULT_HEIGHT,
        OCTO_WINDOW_DEFAULT_TITLE,
        OCTO_WINDOW_DEFAULT_FULLSCREEN);
}

void octo_main_loop(GLFWwindow *handle)
{
    log_debug("octo_mail_loop(): started");

    while (!glfwWindowShouldClose(handle))
    {
        glfwPollEvents();
    }

    log_debug("octo_mail_loop(): ended");
}

void octo_shutdown(GLFWwindow *handle)
{
    log_debug("octo_shutdown()");

    glfwDestroyWindow(handle);
    glfwTerminate();

    log_debug("\t* completed shutdown - bye!\n\n");
}