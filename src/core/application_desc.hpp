#pragma once

#include "defines.hpp"

#include <GLFW/glfw3.h>
#include <sstream>
#include <string>

namespace octo {
    struct ApplicationDesc {
        std::string name = "octo";
        uint32_t width = 1280;
        uint32_t height = 720;
        bool fullscreen = false;
        GLFWwindow* windowHandle;
    };

    // std::string DumpApplicationDesc(ApplicationDesc appDesc) {
    //     std::stringstream ss;

    //     ss << "{ ";
    //     ss << "name: " << appDesc.name;
    //     ss << ", width: " << appDesc.width;
    //     ss << ", height: " << appDesc.height;
    //     ss << ", fullscreen: " << appDesc.fullscreen ? "true" : "false";
    //     ss << ", windowHandle: " << appDesc.windowHandle;
    //     ss << " }";

    //     return ss.str();
    // }
}