#include "../include/renderer/renderer.hpp"
#include "../include/core/logger.hpp"
// #include "../include/core/definitions.hpp"

// #include <vulkan/vulkan.h>

#include <stdexcept>
#include <cstdlib>

namespace octo
{
    Renderer::Renderer(Context *context) : context{context}
    {
        LogDebug("octo::Renderer");

        if (!context->windowHandle)
        {
            throw std::runtime_error("Renderer: missing WindowInfo.windowHandle");
        }

        initVulkan();
    }

    Renderer::~Renderer()
    {
        cleanup();
    }

    void Renderer::render()
    {
    }

    void Renderer::resize(int width, int height)
    {
        LogDebug("octo::Renderer::resize: not implemented yet");
    }

    void Renderer::initVulkan()
    {
        LogDebug("octo::Renderer::initVulkan");
        createInstance();
    }

    void Renderer::cleanup()
    {
        LogDebug("octo::Renderer::cleanup");
    }

    void Renderer::createInstance()
    {
        LogDebug("octo::Renderer::createInstance");

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = context->name.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(
            OCTO_VERSION_MAJOR,
            OCTO_VERSION_MINOR,
            OCTO_VERSION_PATCH);
        appInfo.pEngineName = OCTO_PRODUCT_NAME;
        appInfo.engineVersion = VK_MAKE_VERSION(
            OCTO_VERSION_MAJOR,
            OCTO_VERSION_MINOR,
            OCTO_VERSION_PATCH);
        appInfo.apiVersion = OCTO_VULKAN_API_VERSION;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            throw std::runtime_error("octo::Renderer: failed to create instance");
        }
    }
}
