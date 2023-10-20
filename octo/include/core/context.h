#pragma once

#include "defines.h"

#include <GLFW/glfw3.h>
#include <string>

typedef struct Context
{
    const char *applicationName;
    const char *applicationVersion;
    const char *engineName = PRODUCT_NAME;
    const char *engineVersion = OCTO_VERSION;
    u32 windowWidth = 1280;
    u32 windowHeight = 720;
    bool fullscreen = false;
    int inputDownTimeThreshold = 250;
    // GLFWwindow *windowHandle;
} Context;
