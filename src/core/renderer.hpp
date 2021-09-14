#pragma once

#include "window.hpp"
#include "context.hpp"
#include "vertex.hpp"

#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <optional>

namespace octo {
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
        Renderer(Context*);
        ~Renderer();

        void render();
        void resize();

        void cleanup();

        void createBuffer(
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties,
            VkBuffer& buffer,
            VkDeviceMemory& bufferMemory
        );

    protected:

    private:
        bool _framebufferResized = false;

        // std::string _applicationName;
        // GLFWwindow* _windowHandle;
        Context* _context;

        VkInstance _instance;
        VkDebugUtilsMessengerEXT _debugMessenger;
        VkSurfaceKHR _surface;

        VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
        VkDevice _device;

        VkQueue _graphicsQueue;
        VkQueue _presentQueue;

        VkSwapchainKHR _swapChain;
        std::vector<VkImage> _swapChainImages;
        VkFormat _swapChainImageFormat;
        VkExtent2D _swapChainExtent;
        std::vector<VkImageView> _swapChainImageViews;
        std::vector<VkFramebuffer> _swapChainFramebuffers;

        VkRenderPass _renderPass;
        VkPipelineLayout _pipelineLayout;
        VkPipeline _graphicsPipeline;

        VkCommandPool _commandPool;
        std::vector<VkCommandBuffer> _commandBuffers;

        std::vector<VkSemaphore> _imageAvailableSemaphores;
        std::vector<VkSemaphore> _renderFinishedSemaphores;
        std::vector<VkFence> _inFlightFences;
        std::vector<VkFence> _imagesInFlight;
        size_t _currentFrame = 0;

        std::vector<Vertex> _vertices = {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };

        VkBuffer _vertexBuffer;
        VkDeviceMemory _vertexBufferMemory;

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
        uint32_t _findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void _copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

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
        void _createVertexBuffer();
        void _createCommandBuffers();
        void _createSyncObjects();

        void _drawFrame();
    };
}