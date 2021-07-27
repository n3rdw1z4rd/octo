#include "renderer.hpp"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <vector>

namespace octo
{
    Renderer::Renderer() {}

    Renderer::~Renderer() {}

    bool Renderer::init(GLFWwindow *window, uint32_t width, uint32_t height)
    {
        uint32_t extensionCount = 0;
        uint32_t requiredInstanceExtensionCount = 0;
        const char **requiredInstanceExtensions;

        { // log extensions info:
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            spdlog::debug("Renderer: supported extension count: {}", extensionCount);

            std::vector<VkExtensionProperties> extensions(extensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

            spdlog::debug("Renderer: available extensions:");

            for (const auto &extension : extensions)
            {
                spdlog::debug("\t{}", extension.extensionName);
            }
            
            requiredInstanceExtensions = glfwGetRequiredInstanceExtensions(&requiredInstanceExtensionCount);

            spdlog::debug("Renderer: required extension count: {}", requiredInstanceExtensionCount);
            spdlog::debug("Renderer: required extensions:");

            for (int i = 0; i < (int)requiredInstanceExtensionCount; i++)
            {
                spdlog::debug("\t{}", requiredInstanceExtensions[i]);
            }
        }

        { // create instance:
            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = PROJECT_NAME;
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.pEngineName = PROJECT_NAME;
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_0;

            VkInstanceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;
            createInfo.enabledExtensionCount = requiredInstanceExtensionCount;
            createInfo.ppEnabledExtensionNames = requiredInstanceExtensions;
            createInfo.enabledLayerCount = 0;

            if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
            {
                spdlog::error("Renderer: failed to create instance");
                return false;
            }
        }

        { // select gpu:
            uint32_t gpuCount;

            if (vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr) != VK_SUCCESS && gpuCount > 0)
            {
                spdlog::error("Renderer: failed to enumerate gpuCount");
                return false;
            }

            VkPhysicalDevice *gpus = (VkPhysicalDevice *)malloc(sizeof(VkPhysicalDevice) * gpuCount);

            if (vkEnumeratePhysicalDevices(instance, &gpuCount, gpus) != VK_SUCCESS)
            {
                spdlog::error("Renderer: failed to allocate gpu devices");
                return false;
            }

            int useGpu = 0;

            for (int i = 0; i < (int)gpuCount; i++)
            {
                VkPhysicalDeviceProperties properties;
                vkGetPhysicalDeviceProperties(gpus[i], &properties);

                if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                {
                    useGpu = i;
                    break;
                }
            }

            physicalDevice = gpus[useGpu];
            free(gpus);
        }

        { // select queue family:
            uint32_t count;

            vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);
            VkQueueFamilyProperties *queues = (VkQueueFamilyProperties *)malloc(sizeof(VkQueueFamilyProperties) * count);
            vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, queues);

            for (uint32_t i = 0; i < count; i++)
            {
                if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    queueFamily = i;
                    break;
                }
            }

            free(queues);

            if (queueFamily == (uint32_t)-1)
            {
                spdlog::error("Renderer: invalid graphics queue family");
                return false;
            }
        }

        { // create device:
            int deviceExtensionCount = 1;
            const char *deviceExtensions[] = {"VK_KHR_swapchain"};
            const float queuePriority[] = {1.0f};

            VkDeviceQueueCreateInfo queueInfo[1] = {};
            queueInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo[0].queueFamilyIndex = queueFamily;
            queueInfo[0].queueCount = 1;
            queueInfo[0].pQueuePriorities = queuePriority;

            VkDeviceCreateInfo create_info = {};
            create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            create_info.queueCreateInfoCount = sizeof(queueInfo) / sizeof(queueInfo[0]);
            create_info.pQueueCreateInfos = queueInfo;
            create_info.enabledExtensionCount = deviceExtensionCount;
            create_info.ppEnabledExtensionNames = deviceExtensions;

            if (vkCreateDevice(physicalDevice, &create_info, allocatorCallbacks, &device) != VK_SUCCESS)
            {
                spdlog::error("Renderer: could not create device");
                return false;
            }

            // vkGetDeviceQueue(device, queueFamily, 0, &queue);
        }

        { // create descriptor pool:
            VkDescriptorPoolSize pool_sizes[] =
                {
                    {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
                    {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                    {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
                    {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
                    {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
                    {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
                    {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
                    {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
                    {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                    {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                    {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

            VkDescriptorPoolCreateInfo pool_info = {};
            pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            pool_info.maxSets = 1000 * ARRAYSIZE(pool_sizes);
            pool_info.poolSizeCount = (uint32_t)ARRAYSIZE(pool_sizes);
            pool_info.pPoolSizes = pool_sizes;

            if (vkCreateDescriptorPool(device, &pool_info, allocatorCallbacks, &descriptorPool) != VK_SUCCESS)
            {
                spdlog::error("Renderer: could not create descriptor pool");
                return false;
            }
        }

        { // create surface:
            if (glfwCreateWindowSurface(instance, window, allocatorCallbacks, &surface) != VK_SUCCESS)
            {
                spdlog::error("Renderer: could not create surface");
                return false;
            }
        }

        { // create framebuffers:
            int w, h;
            glfwGetFramebufferSize(window, &w, &h);

            VkBool32 res;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamily, surface, &res);

            if (res != VK_TRUE)
            {
                spdlog::error("Renderer: no WSI support");
                return false;
            }
        }

        return true;
    }

    void Renderer::destroy()
    {
        if (descriptorPool)
        {
            vkDestroyDescriptorPool(device, descriptorPool, allocatorCallbacks);
        }

        if (device)
        {
            vkDestroyDevice(device, allocatorCallbacks);
        }

        if (instance)
        {
            vkDestroyInstance(instance, allocatorCallbacks);
        }

        if (instance != nullptr)
        {
            vkDestroyInstance(instance, nullptr);
        }
    }
}