#pragma once

#include "defines.hpp"
#include "renderer.hpp"
#include "../input/input.hpp"
#include "../third_party/json.hpp"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
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
        Renderer renderer{};
        // Input input{};

    public:
        nlohmann::json settings;

        Application(int argc, char **argv);
        ~Application();

        nlohmann::json getDefaultSettings();
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

        bool init();
        void start();
        void stop();
    };
}