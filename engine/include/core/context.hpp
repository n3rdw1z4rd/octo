#pragma once

#include "defines.hpp"

#include <GLFW/glfw3.h>
#include <sstream>
#include <string>

namespace octo
{
    struct Context
    {
        std::string name = ENGINE_NAME;
        std::string version = VERSION;
        uint32_t width = 1280;
        uint32_t height = 720;
        bool fullscreen = false;
        int inputDownTimeThreshold = 250;
        GLFWwindow *windowHandle;
    };
}