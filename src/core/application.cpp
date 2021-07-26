#include "application.hpp"

#include <spdlog/spdlog.h>
#include <fstream>

namespace octo
{
    Application::Application(int argc, char **argv)
    {
        std::string configFilePath{DEFAULT_CONFIG_PATH};

        // TODO: parse argv
        settings = loadJsonFile(configFilePath);

        if (settings == nullptr)
        {
            spdlog::warn("Application: config file '{}' not found, creating one based on defaults...", configFilePath);
            configFilePath = DEFAULT_CONFIG_PATH;
            settings = getDefaultSettings();
            saveJsonFile(configFilePath, settings);
        }

        width = settings["application"]["width"].get<int>();
        height = settings["application"]["height"].get<int>();
        fullscreen = settings["application"]["fullscreen"].get<bool>();
    }

    Application::~Application() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    nlohmann::json Application::getDefaultSettings()
    {
        nlohmann::json defaultSettings = {
            {"application", {
                {"title", PROJECT_NAME},
                {"fullscreen", false},
                {"width", 800},
                {"height", 600}
            }}
        };

        return defaultSettings;
    }

    nlohmann::json Application::loadJsonFile(const std::string &path)
    {
        std::ifstream file(path.c_str());

        if (!file.good())
        {
            spdlog::warn("loadJsonFile: cannot find {}", path);
            return nullptr;
        }

        nlohmann::json json = nlohmann::json::parse(file);
        file.close();

        spdlog::debug("loadJsonFile: loaded {}: {}", path, json.dump(2));

        return json;
    }

    bool Application::saveJsonFile(const std::string &path, nlohmann::json json)
    {
        if (path == "")
        {
            spdlog::warn("saveJsonFile: missing valid path");
            return false;
        }

        std::ofstream *h = new std::ofstream(path.c_str(), std::ios::trunc);
        (*h) << json.dump(4);
        h->close();

        return true;
    }

    void Application::framebufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        auto _window = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
        _window->framebufferResized = true;
        _window->width = width;
        _window->height = height;
    }

    void Application::init() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(
            settings["application"]["width"],
            settings["application"]["height"],
            settings["application"]["title"].get<std::string>().c_str(),
            nullptr, nullptr // TODO: fullscreen?
        );

        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        spdlog::debug("Application: supported extension count: {}", extensionCount);
    }

    void Application::start() {
        while(!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void Application::stop() {
        glfwSetWindowShouldClose(window, 1);
    }
}