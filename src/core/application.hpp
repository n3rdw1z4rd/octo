#pragma once

#include "defines.hpp"
#include "logger.hpp"

#include <GLFW/glfw3.h>
#include <string>
#include <functional>

class Application {
public:
    Application();
    ~Application();

    bool initialize(std::string title, uint32_t width, uint32_t height);
    void run(std::function<void(bool)> update = nullptr);

    GLFWwindow* getWindowHandle() { return windowHandle; }
private:
    std::string windowTitle;
    uint32_t windowWidth;
    uint32_t windowHeight;
    bool isInitialized = false;
    bool isRunning = false;
    bool framebufferResized = false;

    GLFWwindow* windowHandle;

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        LogDebug("framebufferResizeCallback:", width, height);
        auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        app->framebufferResized = true;
    }
};