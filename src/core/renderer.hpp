#pragma once

#include "defines.hpp"

#include <vulkan/vulkan.h>

namespace octo
{
    class Renderer
    {
        VkInstance instance = nullptr;
        VkPhysicalDevice physicalDevice = nullptr;
        uint32_t queueFamily;
        VkAllocationCallbacks *allocatorCallbacks = nullptr;
        VkDevice device;
        VkQueue queue;
        VkDescriptorPool descriptorPool;
        VkSurfaceKHR surface;

    public:
        Renderer();
        ~Renderer();

        bool init(GLFWwindow *window, uint32_t width, uint32_t height);
        void destroy();
    };
}