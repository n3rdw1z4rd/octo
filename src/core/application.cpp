#include "application.hpp"

Application::Application() { }

Application::~Application() {
    LogDebug("Application: terminating...");

    glfwDestroyWindow(windowHandle);
    glfwTerminate();

    LogInfo("good bye!\n");
}

bool Application::initialize(std::string title, uint32_t width, uint32_t height) {
    LogDebug("Application::initialize:", title, width, height);

    if (isInitialized) {
        LogWarn("Application::initialize: already initialized");
        return true;
    }

    windowTitle = title;
    windowWidth = width;
    windowHeight = height;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    windowHandle = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);

    if (!windowHandle) {
        LogError("Application::initialize: failed to initialize (GLFWwindow*)");
        return false;
    }

    glfwSetWindowUserPointer(windowHandle, this);
    glfwSetFramebufferSizeCallback(windowHandle, framebufferResizeCallback);

    isInitialized = true;
    return true;
}

void Application::run(std::function<void(bool)> update) {
    LogDebug("Application::run");

    if (isRunning) {
        LogWarn("Application::run: already running");
        return;
    }

    isRunning = true;

    while (isRunning) {
        glfwPollEvents();

        if (update) {
            update(framebufferResized);
            framebufferResized = false;
        }

        isRunning = !glfwWindowShouldClose(windowHandle);
    }
}