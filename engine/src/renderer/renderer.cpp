#include "../../include/renderer/renderer.hpp"
#include "../../include/core/logger.hpp"

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

#if defined(OCTO_PLATFORM_MACOS)
        // required for macos for Vulkan API 1.3:
        std::vector<const char *> requiredExtensions;

        for (uint32_t i = 0; i < glfwExtensionCount; i++)
        {
            requiredExtensions.emplace_back(glfwExtensions[i]);
        }

        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();
#endif

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            throw std::runtime_error("octo::Renderer: failed to create instance");
        }
    }
}
