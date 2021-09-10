#pragma once

#include "core/defines.hpp"

#include <GLFW/glfw3.h>
#include <shaderc/shaderc.hpp>

#include <string>
#include <vector>
#include <optional>

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool initialize(const std::string name, GLFWwindow* handle);

    void render(bool resized);

    void cleanup();

    std::vector<uint32_t> compileShaderSource(
        const std::string& name,
        const std::string& source,
        shaderc_shader_kind kind,
        const std::vector<std::pair<std::string, std::string>>& definitions = {}
    );

protected:

private:
    bool initialized = false;
    bool framebufferResized = false;

    std::string applicationName;
    GLFWwindow* windowHandle;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

    void _cleanupSwapChain();
    bool _checkValidationLayerSupport();
    std::vector<const char*> _getRequiredExtensions();
    void _populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    QueueFamilyIndices _findQueueFamilies(VkPhysicalDevice device);
    bool _checkDeviceExtensionSupport(VkPhysicalDevice device);
    SwapChainSupportDetails _querySwapChainSupport(VkPhysicalDevice device);
    bool _isDeviceSuitable(VkPhysicalDevice device);
    VkSurfaceFormatKHR _chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR _chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D _chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkShaderModule _createShaderModule(const std::vector<uint32_t>& code);
    void _recreateSwapChain();

    void _createInstance();
    void _setupDebugMessenger();
    void _createSurface();
    void _pickPhysicalDevice();
    void _createLogicalDevice();
    void _createSwapChain();
    void _createImageViews();
    void _createRenderPass();
    void _createGraphicsPipeline();
    void _createFramebuffers();
    void _createCommandPool();
    void _createCommandBuffers();
    void _createSyncObjects();

    void _drawFrame();
};