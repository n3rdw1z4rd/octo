#pragma once

#include "defines.hpp"

#include <GLFW/glfw3.h>
#include <sstream>
#include <string>

namespace octo {
    struct Context {
        std::string name = "octo";
        uint32_t width = 1280;
        uint32_t height = 720;
        bool fullscreen = false;
        int inputDownTimeThreshold = 250;
        GLFWwindow* windowHandle;
    };
}