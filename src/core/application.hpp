#pragma once

#include "defines.hpp"
#include "../input/input.hpp"

#include "../third_party/json.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace octo
{
    class Application
    {
        int width;
        int height;
        bool fullscreen;
        bool framebufferResized = false;

        GLFWwindow *window;
        Input *input;

    public:
        nlohmann::json settings;

        Application(int argc, char **argv);
        ~Application();

        nlohmann::json getDefaultSettings();
        nlohmann::json loadJsonFile(const std::string &path);
        bool saveJsonFile(const std::string &path, nlohmann::json json);

        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

        void init();
        void start();
        void stop();
    };
}