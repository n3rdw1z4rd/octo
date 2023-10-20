// #include "../include/renderer/renderer.hpp"
// #include "../include/core/logger.hpp"

// #include <stdexcept>
// #include <cstdlib>
// #include <cstring>
// #include <optional>
// #include <set>

// namespace octo
// {
//     VkResult CreateDebugUtilsMessengerEXT(
//         VkInstance instance,
//         const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
//         const VkAllocationCallbacks *pAllocator,
//         VkDebugUtilsMessengerEXT *pDebugMessenger)
//     {
//         LogDebug("octo::Renderer::CreateDebugUtilsMessengerEXT");

//         auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

//         if (func != nullptr)
//         {
//             return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
//         }
//         else
//         {
//             return VK_ERROR_EXTENSION_NOT_PRESENT;
//         }
//     }

//     void DestroyDebugUtilsMessengerEXT(
//         VkInstance instance,
//         VkDebugUtilsMessengerEXT debugMessenger,
//         const VkAllocationCallbacks *pAllocator)
//     {
//         LogDebug("octo::Renderer::DestroyDebugUtilsMessengerEXT");

//         auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

//         if (func != nullptr)
//         {
//             func(instance, debugMessenger, pAllocator);
//         }
//     }

//     struct QueueFamilyIndices
//     {
//         std::optional<uint32_t> graphicsFamily;
//         std::optional<uint32_t> presentFamily;

//         bool isComplete()
//         {
//             return graphicsFamily.has_value() && presentFamily.has_value();
//         }
//     };

//     struct SwapChainSupportDetails
//     {
//         VkSurfaceCapabilitiesKHR capabilities;
//         std::vector<VkSurfaceFormatKHR> formats;
//         std::vector<VkPresentModeKHR> presentModes;
//     };

//     struct UniformBufferObject
//     {
//         alignas(16) glm::mat4 model;
//         alignas(16) glm::mat4 view;
//         alignas(16) glm::mat4 proj;
//     };

//     Renderer::Renderer(Context *context) : context{context}
//     {
//         LogDebug("octo::Renderer");

//         if (!context->windowHandle)
//         {
//             throw std::runtime_error("octo::Renderer: missing WindowInfo.windowHandle");
//         }

//         initVulkan();
//     }

//     Renderer::~Renderer()
//     {
//         cleanup();
//     }

//     void Renderer::render()
//     {
//     }

//     void Renderer::resize(int width, int height)
//     {
//         LogWarn("octo::Renderer::resize: not implemented yet");
//     }

//     void Renderer::initVulkan()
//     {
//         LogDebug("octo::Renderer::initVulkan");
//         logSupportedExtensions();
//         createInstance();
//         setupDebugMessenger();
//         pickPhysicalDevice();
//     }

//     void Renderer::cleanup()
//     {
//         LogDebug("octo::Renderer::cleanup");

//         vkDestroyInstance(instance, nullptr);
//     }

//     void Renderer::createInstance()
//     {
//         LogDebug("octo::Renderer::createInstance");

//         if (enableValidationLayers && !checkValidationLayerSupport())
//         {
//             throw std::runtime_error("octo::Renderer::recreateSwapChain: validation layers requested, but not available!");
//         }

//         VkApplicationInfo appInfo{};
//         appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//         appInfo.pApplicationName = context->name.c_str();
//         appInfo.applicationVersion = VK_MAKE_VERSION(
//             OCTO_VERSION_MAJOR,
//             OCTO_VERSION_MINOR,
//             OCTO_VERSION_PATCH);
//         appInfo.pEngineName = OCTO_PRODUCT_NAME;
//         appInfo.engineVersion = VK_MAKE_VERSION(
//             OCTO_VERSION_MAJOR,
//             OCTO_VERSION_MINOR,
//             OCTO_VERSION_PATCH);
//         appInfo.apiVersion = OCTO_VULKAN_API_VERSION;

//         VkInstanceCreateInfo createInfo{};
//         createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//         createInfo.pApplicationInfo = &appInfo;

//         // uint32_t glfwExtensionCount = 0;
//         // const char **glfwExtensions;

//         // glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

//         // createInfo.enabledExtensionCount = glfwExtensionCount;
//         // createInfo.ppEnabledExtensionNames = glfwExtensions;
//         // createInfo.enabledLayerCount = 0;

//         auto extensions = getRequiredExtensions();
//         createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
//         createInfo.ppEnabledExtensionNames = extensions.data();

//         VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
//         if (enableValidationLayers)
//         {
//             createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
//             createInfo.ppEnabledLayerNames = validationLayers.data();

//             populateDebugMessengerCreateInfo(debugCreateInfo);
//             createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
//         }
//         else
//         {
//             createInfo.enabledLayerCount = 0;

//             createInfo.pNext = nullptr;
//         }

// #if defined(OCTO_PLATFORM_MACOS)
//         // required for macos for Vulkan API 1.3:
//         std::vector<const char *> requiredExtensions;

//         for (uint32_t i = 0; i < glfwExtensionCount; i++)
//         {
//             requiredExtensions.emplace_back(glfwExtensions[i]);
//         }

//         requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
//         createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
//         createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
//         createInfo.ppEnabledExtensionNames = requiredExtensions.data();
// #endif

//         if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
//         {
//             throw std::runtime_error("octo::Renderer: failed to create instance");
//         }
//     }

//     bool Renderer::checkValidationLayerSupport()
//     {
//         LogDebug("octo::Renderer::checkValidationLayerSupport");

//         uint32_t layerCount;
//         vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

//         std::vector<VkLayerProperties> availableLayers(layerCount);
//         vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

//         for (const char *layerName : validationLayers)
//         {
//             bool layerFound = false;

//             for (const auto &layerProperties : availableLayers)
//             {
//                 if (strcmp(layerName, layerProperties.layerName) == 0)
//                 {
//                     layerFound = true;
//                     break;
//                 }
//             }

//             if (!layerFound)
//             {
//                 return false;
//             }
//         }

//         return true;
//     }

//     void Renderer::logSupportedExtensions()
//     {
//         uint32_t extensionCount = 0;
//         vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

//         std::vector<VkExtensionProperties> extensions(extensionCount);

//         vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

//         LogDebug("octo::Renderer::logSupportedExtensions:");

//         for (const auto &extension : extensions)
//         {
//             LogDebug("\t", extension.extensionName);
//         }
//     }

//     std::vector<const char *> Renderer::getRequiredExtensions()
//     {
//         LogDebug("octo::Renderer::getRequiredExtensions");

//         uint32_t glfwExtensionCount = 0;
//         const char **glfwExtensions;
//         glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

//         std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

//         if (enableValidationLayers)
//         {
//             extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//         }

//         return extensions;
//     }

//     void Renderer::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
//     {
//         LogDebug("octo::Renderer::populateDebugMessengerCreateInfo");

//         createInfo = {};
//         createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
//         createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
//         createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
//         createInfo.pfnUserCallback = debugCallback;
//     }

//     VKAPI_ATTR VkBool32 VKAPI_CALL Renderer::debugCallback(
//         VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
//         VkDebugUtilsMessageTypeFlagsEXT messageType,
//         const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
//         void *pUserData)
//     {
//         switch (messageType)
//         {
//         // case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
//         //     LogDebug("octo::Renderer::debugCallback: ", pCallbackData->pMessage);
//         //     break;
//         case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
//             LogInfo("octo::Renderer::debugCallback: ", pCallbackData->pMessage);
//             break;
//         case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
//             LogWarn("octo::Renderer::debugCallback: ", pCallbackData->pMessage);
//             break;
//         case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
//             LogError("octo::Renderer::debugCallback: ", pCallbackData->pMessage);
//             break;
//         default:
//             break;
//         }

//         return VK_FALSE;
//     }

//     void Renderer::setupDebugMessenger()
//     {
//         LogDebug("octo::Renderer::setupDebugMessenger");

//         if (!enableValidationLayers)
//             return;

//         VkDebugUtilsMessengerCreateInfoEXT createInfo;
//         populateDebugMessengerCreateInfo(createInfo);

//         if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
//         {
//             throw std::runtime_error("octo::Renderer::setupDebugMessenger: failed to set up debug messenger!");
//         }
//     }

//     void Renderer::pickPhysicalDevice()
//     {
//         LogDebug("octo::Renderer::pickPhysicalDevice");

//         uint32_t deviceCount = 0;
//         vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

//         if (deviceCount == 0)
//         {
//             throw std::runtime_error("octo::Renderer::pickPhysicalDevice: failed to find GPUs with Vulkan support!");
//         }

//         LogDebug("octo::Renderer::pickPhysicalDevice: deviceCount: ", deviceCount);

//         std::vector<VkPhysicalDevice> devices(deviceCount);
//         vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

//         for (const auto &device : devices)
//         {
//             LogDebug("device: ", device);

//             if (isDeviceSuitable(device))
//             {
//                 physicalDevice = device;
//                 msaaSamples = getMaxUsableSampleCount();
//                 break;
//             }
//         }

//         if (physicalDevice == VK_NULL_HANDLE)
//         {
//             throw std::runtime_error("octo::Renderer::pickPhysicalDevice: failed to find a suitable GPU!");
//         }
//     }

//     SwapChainSupportDetails Renderer::querySwapChainSupport(VkPhysicalDevice device)
//     {
//         LogDebug("octo::Renderer::querySwapChainSupport");

//         SwapChainSupportDetails details;

//         vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

//         uint32_t formatCount;
//         vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

//         if (formatCount != 0)
//         {
//             details.formats.resize(formatCount);
//             vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
//         }

//         uint32_t presentModeCount;
//         vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

//         if (presentModeCount != 0)
//         {
//             details.presentModes.resize(presentModeCount);
//             vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
//         }

//         return details;
//     }

//     bool Renderer::isDeviceSuitable(VkPhysicalDevice device)
//     {
//         LogDebug("octo::Renderer::isDeviceSuitable");

//         QueueFamilyIndices indices = findQueueFamilies(device);

//         bool extensionsSupported = checkDeviceExtensionSupport(device);

//         bool swapChainAdequate = false;
//         if (extensionsSupported)
//         {
//             SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
//             swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
//         }

//         VkPhysicalDeviceFeatures supportedFeatures;
//         vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

//         return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
//     }

//     VkSampleCountFlagBits Renderer::getMaxUsableSampleCount()
//     {
//         LogDebug("octo::Renderer::getMaxUsableSampleCount");

//         VkPhysicalDeviceProperties physicalDeviceProperties;
//         vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

//         VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
//         if (counts & VK_SAMPLE_COUNT_64_BIT)
//         {
//             return VK_SAMPLE_COUNT_64_BIT;
//         }
//         if (counts & VK_SAMPLE_COUNT_32_BIT)
//         {
//             return VK_SAMPLE_COUNT_32_BIT;
//         }
//         if (counts & VK_SAMPLE_COUNT_16_BIT)
//         {
//             return VK_SAMPLE_COUNT_16_BIT;
//         }
//         if (counts & VK_SAMPLE_COUNT_8_BIT)
//         {
//             return VK_SAMPLE_COUNT_8_BIT;
//         }
//         if (counts & VK_SAMPLE_COUNT_4_BIT)
//         {
//             return VK_SAMPLE_COUNT_4_BIT;
//         }
//         if (counts & VK_SAMPLE_COUNT_2_BIT)
//         {
//             return VK_SAMPLE_COUNT_2_BIT;
//         }

//         return VK_SAMPLE_COUNT_1_BIT;
//     }

//     QueueFamilyIndices Renderer::findQueueFamilies(VkPhysicalDevice device)
//     {
//         LogDebug("octo::Renderer::findQueueFamilies");

//         QueueFamilyIndices indices;

//         LogDebug("findQueueFamilies: 1");
//         uint32_t queueFamilyCount = 0;
//         vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

//         LogDebug("findQueueFamilies: 2");
//         std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
//         vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

//         LogDebug("findQueueFamilies: 3");
//         int i = 0;
//         for (const auto &queueFamily : queueFamilies)
//         {
//             LogDebug("findQueueFamilies: 4");
            
//             LogDebug("queueFamily.queueFlags: ", queueFamily.queueFlags);
//             LogDebug("queueFamily.queueCount: ", queueFamily.queueCount);
//             LogDebug("queueFamily.timestampValidBits: ", queueFamily.timestampValidBits);
//             // LogDebug("queueFamily.minImageTransferGranularity: ", queueFamily.minImageTransferGranularity);

//             if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
//             {
//                 indices.graphicsFamily = i;
//             }

//             LogDebug("findQueueFamilies: 5");
//             VkBool32 presentSupport = false;

//             LogDebug("vkGetPhysicalDeviceSurfaceSupportKHR: device: ", device);
//             LogDebug("vkGetPhysicalDeviceSurfaceSupportKHR: i: ", i);
//             LogDebug("vkGetPhysicalDeviceSurfaceSupportKHR: surface: ", surface);
//             LogDebug("vkGetPhysicalDeviceSurfaceSupportKHR: &presentSupport: ", &presentSupport);
//             VkResult vkResult = vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
//             LogDebug("findQueueFamilies: vkResult: ", vkResult);
//             LogDebug("findQueueFamilies: 6");

//             if (presentSupport)
//             {
//                 indices.presentFamily = i;
//             }

//             if (indices.isComplete())
//             {
//                 break;
//             }

//             i++;
//         }

//         LogDebug("findQueueFamilies: DONE");
//         return indices;
//     }

//     bool Renderer::checkDeviceExtensionSupport(VkPhysicalDevice device)
//     {
//         LogDebug("octo::Renderer::checkDeviceExtensionSupport");

//         uint32_t extensionCount;
//         vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

//         std::vector<VkExtensionProperties> availableExtensions(extensionCount);
//         vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

//         std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

//         for (const auto &extension : availableExtensions)
//         {
//             requiredExtensions.erase(extension.extensionName);
//         }

//         return requiredExtensions.empty();
//     }

// }
