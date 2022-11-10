#pragma once

#include "defines.hpp"
#include "logger.hpp"
#include "utils.hpp"

#include <string>
#include <GLFW/glfw3.h>

namespace octo
{
    typedef GLFWwindow WindowHandle;

    typedef struct
    {
        WindowHandle *windowHandle;
        std::string windowTitle;
        unsigned int windowWidth;
        unsigned int windowHeight;
        bool fullscreen;
    } Context;

    Context *create_default_context();
    bool init(Context *);
    void shutdown(Context *);
    void main_loop(Context *);
}