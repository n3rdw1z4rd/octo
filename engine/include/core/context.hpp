#pragma once

#include "definitions.hpp"

#include <GLFW/glfw3.h>
#include <sstream>
#include <string>

namespace octo
{
    struct Context
    {
        std::string name = OCTO_PRODUCT_NAME;
        std::string version = OCTO_VERSION;
        uint32_t width = 1280;
        uint32_t height = 720;
        bool fullscreen = false;
        int inputDownTimeThreshold = 250;
        GLFWwindow *windowHandle;
    };
}