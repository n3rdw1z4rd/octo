#pragma once

#include "defines.hpp"

#include <GLFW/glfw3.h>
#include <sstream>
#include <string>

namespace octo
{
    struct Context
    {
        std::string applicationName;
        std::string applicationVersion;
        const char *engineName = PRODUCT_NAME;
        const char *engineVersion = OCTO_VERSION;
        uint32_t windowWidth = 1280;
        uint32_t windowHeight = 720;
        bool fullscreen = false;
        int inputDownTimeThreshold = 250;
        // GLFWwindow *windowHandle;
    };
}