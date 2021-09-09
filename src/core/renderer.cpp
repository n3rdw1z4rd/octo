#include "renderer.hpp"
// #include "application.hpp"
#include "shader.hpp"
#include "../shaders/depth_shader.hpp"

#include <cstring>
#include <vector>
#include <optional>
#include <set>
#include <algorithm>

static struct render_state {
    GLFWwindow* window;
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkSampleCountFlagBits msaa_samples = VK_SAMPLE_COUNT_1_BIT;
    VkDevice device;
    VkQueue graphics_queue;
    VkQueue present_queue;
    VkSwapchainKHR swap_chain;
    std::vector<VkImage> swap_chain_images;
    VkFormat swap_chain_image_format;
    VkExtent2D swap_chain_extent;
    std::vector<VkImageView> swap_chain_image_views;
    std::vector<VkFramebuffer> swap_chain_framebuffers;
    VkRenderPass render_pass;
    VkDescriptorSetLayout descriptor_set_layout;
    VkPipelineLayout pipeline_layout;
    VkPipeline graphics_pipeline;

    VkDebugUtilsMessengerEXT debug_messenger;
} state;

struct queue_family_indices {
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> present_family;

    bool is_complete() {
        return graphics_family.has_value() && present_family.has_value();
    }
};

struct swap_chain_support_details {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validation_layers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool enable_validation_layers = false;
#else
const bool enable_validation_layers = true;
#endif

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
    void* p_user_data
) {
    switch (message_severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            // log_debug(p_callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            log_info(p_callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            log_warn(p_callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            log_error(p_callback_data->pMessage);
            break;
        default: break;
    }

    return VK_FALSE;
}

bool check_validation_layer_support() {
    log_debug("check_validation_layer_support");

    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    for (const char* layer_name : validation_layers) {
        bool layer_found = false;

        for (const auto& layer_properties : available_layers) {
            if (strcmp(layer_name, layer_properties.layerName) == 0) {
                layer_found = true;
                break;
            }
        }

        if (!layer_found) {
            return false;
        }
    }

    return true;
}

std::vector<const char*> get_required_extensions() {
    log_debug("get_required_extensions");

    uint32_t extension_count = 0;
    const char** glfw_extensions;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&extension_count);

    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + extension_count);

    if (enable_validation_layers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& create_info) {
    create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = debug_callback;
}

VkResult create_debug_utils_messenger_ext(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* p_create_info,
    const VkAllocationCallbacks* p_allocator,
    VkDebugUtilsMessengerEXT* p_debug_messenger
) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr) {
        return func(instance, p_create_info, p_allocator, p_debug_messenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void destroy_debug_utils_messenger_ext(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debug_messenger,
    const VkAllocationCallbacks* p_allocator
) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != nullptr) {
        func(instance, debug_messenger, p_allocator);
    }
}

bool renderer_create_instance(GLFWwindow* window, application_config config) {
    log_debug("renderer_create_instance");

    state.window = window;

    if (enable_validation_layers && !check_validation_layer_support()) {
        log_error("renderer_create_instance: request validation layers not available");
        return false;
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &appInfo;

    auto extensions = get_required_extensions();
    create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    create_info.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enable_validation_layers) {
        create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        create_info.ppEnabledLayerNames = validation_layers.data();

        populate_debug_messenger_create_info(debugCreateInfo);
        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    } else {
        create_info.enabledLayerCount = 0;

        create_info.pNext = nullptr;
    }

    if (vkCreateInstance(&create_info, nullptr, &state.instance) != VK_SUCCESS) {
        log_error("renderer_create_instance: failed to create instance");
        return false;
    }

    if (enable_validation_layers) {
        VkDebugUtilsMessengerCreateInfoEXT create_info_ex;
        populate_debug_messenger_create_info(create_info_ex);

        if (create_debug_utils_messenger_ext(
            state.instance,
            &create_info_ex,
            nullptr,
            &state.debug_messenger
        ) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger");
        }
    }


    return true;
}

bool renderer_create_window_surface() {
    log_debug("renderer_create_window_surface");

    if (glfwCreateWindowSurface(state.instance, state.window, nullptr, &state.surface) != VK_SUCCESS) {
        log_error("renderer_create_window_surface: failed to create window surface");
        return false;
    }

    return true;
}

queue_family_indices find_queue_families(VkPhysicalDevice device) {
    log_debug("find_queue_families");

    queue_family_indices indices;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    int i = 0;

    for (const auto& queue_family : queue_families) {
        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics_family = i;
        }

        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, state.surface, &present_support);

        if (present_support) {
            indices.present_family = i;
        }

        if (indices.is_complete()) {
            break;
        }

        i++;
    }

    return indices;
}

bool check_device_extension_support(VkPhysicalDevice device) {
    log_debug("check_device_extension_support");

    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

    std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());

    for (const auto& extension : available_extensions) {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

swap_chain_support_details query_swap_chain_support(VkPhysicalDevice device) {
    log_debug("query_swap_chain_support");

    swap_chain_support_details details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, state.surface, &details.capabilities);

    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, state.surface, &format_count, nullptr);

    if (format_count != 0) {
        details.formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, state.surface, &format_count, details.formats.data());
    }

    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, state.surface, &present_mode_count, nullptr);

    if (present_mode_count != 0) {
        details.present_modes.resize(present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, state.surface, &present_mode_count, details.present_modes.data());
    }

    return details;
}

bool is_device_suitable(VkPhysicalDevice device) {
    log_debug("is_device_suitable");

    queue_family_indices indices = find_queue_families(device);

    bool extensions_supported = check_device_extension_support(device);
    bool swap_chain_adequate = false;

    if (extensions_supported) {
        swap_chain_support_details swap_chain_support = query_swap_chain_support(device);
        swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.present_modes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.is_complete() && extensions_supported && swap_chain_adequate && supportedFeatures.samplerAnisotropy;
}

VkSampleCountFlagBits get_max_usable_sample_count() {
    VkPhysicalDeviceProperties physical_device_properties;
    vkGetPhysicalDeviceProperties(state.physical_device, &physical_device_properties);

    VkSampleCountFlags counts = physical_device_properties.limits.framebufferColorSampleCounts &
        physical_device_properties.limits.framebufferDepthSampleCounts;

    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

    return VK_SAMPLE_COUNT_1_BIT;
}

bool renderer_create_device() {
    log_debug("renderer_create_device");

    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(state.instance, &device_count, nullptr);

    if (device_count == 0) {
        log_error("renderer_create_device: failed to find any Vulkan supported GPUs");
        return false;
    }

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(state.instance, &device_count, devices.data());

    for (const auto& device : devices) {
        if (is_device_suitable(device)) {
            state.physical_device = device;
            state.msaa_samples = get_max_usable_sample_count();
            break;
        }
    }

    if (state.physical_device == VK_NULL_HANDLE) {
        log_error("renderer_create_device: failed to find a suitable GPU");
        return false;
    }

    queue_family_indices indices = find_queue_families(state.physical_device);

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::set<uint32_t> unique_queue_families = { indices.graphics_family.value(), indices.present_family.value() };

    float queue_priority = 1.0f;
    for (uint32_t queue_family : unique_queue_families) {
        VkDeviceQueueCreateInfo queue_create_info{};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(queue_create_info);
    }

    VkPhysicalDeviceFeatures device_features{};
    device_features.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
    create_info.pQueueCreateInfos = queue_create_infos.data();

    create_info.pEnabledFeatures = &device_features;

    create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
    create_info.ppEnabledExtensionNames = device_extensions.data();

    if (enable_validation_layers) {
        create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        create_info.ppEnabledLayerNames = validation_layers.data();
    } else {
        create_info.enabledLayerCount = 0;
    }

    if (vkCreateDevice(state.physical_device, &create_info, nullptr, &state.device) != VK_SUCCESS) {
        log_error("renderer_create_device: failed to create logical device");
        return false;
    }

    vkGetDeviceQueue(state.device, indices.graphics_family.value(), 0, &state.graphics_queue);
    vkGetDeviceQueue(state.device, indices.present_family.value(), 0, &state.present_queue);

    return true;
}

VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats) {
    log_debug("choose_swap_surface_format");

    for (const auto& available_format : available_formats) {
        if (
            available_format.format == VK_FORMAT_B8G8R8A8_SRGB &&
            available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
            ) {
            return available_format;
        }
    }

    return available_formats[0];
}

VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes) {
    log_debug("choose_swap_present_mode");

    for (const auto& available_present_mode : available_present_modes) {
        if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return available_present_mode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities) {
    log_debug("choose_swap_extent");

    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(state.window, &width, &height);

        VkExtent2D actual_extent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actual_extent;
    }
}

VkImageView create_image_view(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) {
    log_debug("create_image_view");

    VkImageViewCreateInfo view_info{};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.image = image;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = format;
    view_info.subresourceRange.aspectMask = aspectFlags;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = mipLevels;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;

    VkImageView image_view;
    if (vkCreateImageView(state.device, &view_info, nullptr, &image_view) != VK_SUCCESS) {
        log_error("create_image_view: failed to create texture image view");
        throw std::runtime_error("create_image_view: failed to create texture image view");
    }

    return image_view;
}

bool renderer_create_swap_chain() {
    log_debug("renderer_create_swap_chain");

    swap_chain_support_details swap_chain_support = query_swap_chain_support(state.physical_device);

    VkSurfaceFormatKHR surface_format = choose_swap_surface_format(swap_chain_support.formats);
    VkPresentModeKHR present_mode = choose_swap_present_mode(swap_chain_support.present_modes);
    VkExtent2D extent = choose_swap_extent(swap_chain_support.capabilities);

    uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;
    if (swap_chain_support.capabilities.maxImageCount > 0 && image_count > swap_chain_support.capabilities.maxImageCount) {
        image_count = swap_chain_support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = state.surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    queue_family_indices indices = find_queue_families(state.physical_device);
    uint32_t queue_family_indices[] = { indices.graphics_family.value(), indices.present_family.value() };

    if (indices.graphics_family != indices.present_family) {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    create_info.preTransform = swap_chain_support.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;

    if (vkCreateSwapchainKHR(state.device, &create_info, nullptr, &state.swap_chain) != VK_SUCCESS) {
        log_error("renderer_create_swap_chain: failed to create swap chain");
        return false;
    }

    vkGetSwapchainImagesKHR(state.device, state.swap_chain, &image_count, nullptr);
    state.swap_chain_images.resize(image_count);
    vkGetSwapchainImagesKHR(state.device, state.swap_chain, &image_count, state.swap_chain_images.data());

    state.swap_chain_image_format = surface_format.format;
    state.swap_chain_extent = extent;

    state.swap_chain_image_views.resize(state.swap_chain_images.size());

    for (uint32_t i = 0; i < state.swap_chain_images.size(); i++) {
        state.swap_chain_image_views[i] = create_image_view(
            state.swap_chain_images[i],
            state.swap_chain_image_format,
            VK_IMAGE_ASPECT_COLOR_BIT,
            1
        );
    }

    return true;
}

VkFormat find_supported_format(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    log_debug("find_supported_format");

    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(state.physical_device, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("find_supported_format: failed to find supported format");
}

VkFormat find_depth_format() {
    log_debug("find_depth_format");

    return find_supported_format(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

bool renderer_create_render_pass() {
    log_debug("renderer_create_render_pass");

    VkAttachmentDescription color_attachment{};
    color_attachment.format = state.swap_chain_image_format;
    color_attachment.samples = state.msaa_samples;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depth_attachment{};
    depth_attachment.format = find_depth_format();
    depth_attachment.samples = state.msaa_samples;
    depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription color_attachment_resolve{};
    color_attachment_resolve.format = state.swap_chain_image_format;
    color_attachment_resolve.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment_resolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment_resolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment_resolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment_resolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment_resolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment_resolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_ref{};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depth_attachment_ref{};
    depth_attachment_ref.attachment = 1;
    depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference color_attachment_resolve_ref{};
    color_attachment_resolve_ref.attachment = 2;
    color_attachment_resolve_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;
    subpass.pDepthStencilAttachment = &depth_attachment_ref;
    subpass.pResolveAttachments = &color_attachment_resolve_ref;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 3> attachments = { color_attachment, depth_attachment, color_attachment_resolve };
    VkRenderPassCreateInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = static_cast<uint32_t>(attachments.size());
    render_pass_info.pAttachments = attachments.data();
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;
    render_pass_info.dependencyCount = 1;
    render_pass_info.pDependencies = &dependency;

    if (vkCreateRenderPass(state.device, &render_pass_info, nullptr, &state.render_pass) != VK_SUCCESS) {
        log_error("renderer_create_render_pass: failed to create render pass");
        return false;
    }

    return true;
}

VkShaderModule create_shader_module(const std::vector<uint32_t>& code) {
    log_debug("create_shader_module: size: ", code.size());

    VkShaderModuleCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = code.size() * sizeof(uint32_t);
    create_info.pCode = code.data();

    VkShaderModule shader_module;
    VkResult result = vkCreateShaderModule(state.device, &create_info, nullptr, &shader_module);

    if (result != VK_SUCCESS) {
        log_error("result: ", result);
        throw std::runtime_error("create_shader_module: " + get_vkresult_string(result));
    }

    return shader_module;
}

bool renderer_create_graphics_pipeline() {
    log_debug("renderer_create_graphics_pipeline");

    VkDescriptorSetLayoutBinding ubo_layout_binding{};
    ubo_layout_binding.binding = 0;
    ubo_layout_binding.descriptorCount = 1;
    ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    ubo_layout_binding.pImmutableSamplers = nullptr;
    ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutBinding sampler_layout_binding{};
    sampler_layout_binding.binding = 1;
    sampler_layout_binding.descriptorCount = 1;
    sampler_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    sampler_layout_binding.pImmutableSamplers = nullptr;
    sampler_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = { ubo_layout_binding, sampler_layout_binding };
    VkDescriptorSetLayoutCreateInfo layout_info{};
    layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = static_cast<uint32_t>(bindings.size());
    layout_info.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(state.device, &layout_info, nullptr, &state.descriptor_set_layout) != VK_SUCCESS) {
        log_error("renderer_create_graphics_pipeline: failed to create descriptor set layout");
        return false;
    }

    auto vert_shader_code = compile_shader(
        "depth_vert_shader",
        depth_vert_shader,
        shaderc_shader_kind::shaderc_glsl_default_vertex_shader
    );

    auto frag_shader_code = compile_shader(
        "depth_frag_shader",
        depth_frag_shader,
        shaderc_shader_kind::shaderc_glsl_default_fragment_shader
    );

    VkShaderModule vert_shader_module = create_shader_module(vert_shader_code);
    VkShaderModule frag_shader_module = create_shader_module(frag_shader_code);

    VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
    vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vert_shader_stage_info.module = vert_shader_module;
    vert_shader_stage_info.pName = "main";

    VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
    frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_shader_stage_info.module = frag_shader_module;
    frag_shader_stage_info.pName = "main";

    VkPipelineShaderStageCreateInfo shader_stages[] = { vert_shader_stage_info, frag_shader_stage_info };

    VkPipelineVertexInputStateCreateInfo vertex_input_info{};
    vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto binding_description = Vertex::getBindingDescription();
    auto attribute_descriptions = Vertex::getAttributeDescriptions();

    vertex_input_info.vertexBindingDescriptionCount = 1;
    vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size());
    vertex_input_info.pVertexBindingDescriptions = &binding_description;
    vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions.data();

    VkPipelineInputAssemblyStateCreateInfo input_assembly{};
    input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)state.swap_chain_extent.width;
    viewport.height = (float)state.swap_chain_extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = state.swap_chain_extent;

    VkPipelineViewportStateCreateInfo viewport_state{};
    viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = 1;
    viewport_state.pViewports = &viewport;
    viewport_state.scissorCount = 1;
    viewport_state.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = state.msaa_samples;

    VkPipelineDepthStencilStateCreateInfo depth_stencil{};
    depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_stencil.depthTestEnable = VK_TRUE;
    depth_stencil.depthWriteEnable = VK_TRUE;
    depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depth_stencil.depthBoundsTestEnable = VK_FALSE;
    depth_stencil.stencilTestEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState color_blend_attachment{};
    color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo color_blending{};
    color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blending.logicOpEnable = VK_FALSE;
    color_blending.logicOp = VK_LOGIC_OP_COPY;
    color_blending.attachmentCount = 1;
    color_blending.pAttachments = &color_blend_attachment;
    color_blending.blendConstants[0] = 0.0f;
    color_blending.blendConstants[1] = 0.0f;
    color_blending.blendConstants[2] = 0.0f;
    color_blending.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipeline_layout_info{};
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_info.setLayoutCount = 1;
    pipeline_layout_info.pSetLayouts = &state.descriptor_set_layout;

    if (vkCreatePipelineLayout(state.device, &pipeline_layout_info, nullptr, &state.pipeline_layout) != VK_SUCCESS) {
        log_error("renderer_create_graphics_pipeline: failed to create pipeline layout");
        return false;
    }

    VkGraphicsPipelineCreateInfo pipeline_info{};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount = 2;
    pipeline_info.pStages = shader_stages;
    pipeline_info.pVertexInputState = &vertex_input_info;
    pipeline_info.pInputAssemblyState = &input_assembly;
    pipeline_info.pViewportState = &viewport_state;
    pipeline_info.pRasterizationState = &rasterizer;
    pipeline_info.pMultisampleState = &multisampling;
    pipeline_info.pDepthStencilState = &depth_stencil;
    pipeline_info.pColorBlendState = &color_blending;
    pipeline_info.layout = state.pipeline_layout;
    pipeline_info.renderPass = state.render_pass;
    pipeline_info.subpass = 0;
    pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(state.device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &state.graphics_pipeline) != VK_SUCCESS) {
        log_error("renderer_create_graphics_pipeline: failed to create graphics pipeline");
        return false;
    }

    vkDestroyShaderModule(state.device, frag_shader_module, nullptr);
    vkDestroyShaderModule(state.device, vert_shader_module, nullptr);

    return true;
}

void cleanup_swap_chain() {
    log_debug("cleanup_swap_chain");

    // vkDestroyImageView(state.device, depthImageView, nullptr);
    // vkDestroyImage(state.device, depthImage, nullptr);
    // vkFreeMemory(state.device, depthImageMemory, nullptr);

    // vkDestroyImageView(state.device, colorImageView, nullptr);
    // vkDestroyImage(state.device, colorImage, nullptr);
    // vkFreeMemory(state.device, colorImageMemory, nullptr);

    for (auto framebuffer : state.swap_chain_framebuffers) {
        vkDestroyFramebuffer(state.device, framebuffer, nullptr);
    }

    // vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

    vkDestroyPipeline(state.device, state.graphics_pipeline, nullptr);
    vkDestroyPipelineLayout(state.device, state.pipeline_layout, nullptr);
    vkDestroyRenderPass(state.device, state.render_pass, nullptr);

    for (auto image_view : state.swap_chain_image_views) {
        vkDestroyImageView(state.device, image_view, nullptr);
    }

    vkDestroySwapchainKHR(state.device, state.swap_chain, nullptr);

    // for (size_t i = 0; i < state.swap_chain_images.size(); i++) {
    //     vkDestroyBuffer(state.device, uniformBuffers[i], nullptr);
    //     vkFreeMemory(state.device, uniformBuffersMemory[i], nullptr);
    // }

    // vkDestroyDescriptorPool(state.device, descriptorPool, nullptr);
}

void renderer_shutdown() {
    log_debug("renderer_shutdown");
    cleanup_swap_chain();

    // vkDestroySampler(device, textureSampler, nullptr);
    // vkDestroyImageView(device, textureImageView, nullptr);

    // vkDestroyImage(device, textureImage, nullptr);
    // vkFreeMemory(device, textureImageMemory, nullptr);

    vkDestroyDescriptorSetLayout(state.device, state.descriptor_set_layout, nullptr);

    // vkDestroyBuffer(device, indexBuffer, nullptr);
    // vkFreeMemory(device, indexBufferMemory, nullptr);

    // vkDestroyBuffer(device, vertexBuffer, nullptr);
    // vkFreeMemory(device, vertexBufferMemory, nullptr);

    // for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    //     vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
    //     vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
    //     vkDestroyFence(device, inFlightFences[i], nullptr);
    // }

    // vkDestroyCommandPool(device, commandPool, nullptr);

    vkDestroyDevice(state.device, nullptr);

    if (enable_validation_layers) {
        destroy_debug_utils_messenger_ext(state.instance, state.debug_messenger, nullptr);
    }

    vkDestroySurfaceKHR(state.instance, state.surface, nullptr);
    vkDestroyInstance(state.instance, nullptr);
}

std::string get_vkresult_string(VkResult value) {
    std::string str;

    switch (value) {
        case VK_ERROR_DEVICE_LOST: str = "VK_ERROR_DEVICE_LOST"; break;
        case VK_ERROR_EXTENSION_NOT_PRESENT: str = "VK_ERROR_EXTENSION_NOT_PRESENT"; break;
        case VK_ERROR_FEATURE_NOT_PRESENT: str = "VK_ERROR_FEATURE_NOT_PRESENT"; break;
        case VK_ERROR_FORMAT_NOT_SUPPORTED: str = "VK_ERROR_FORMAT_NOT_SUPPORTED"; break;
        case VK_ERROR_FRAGMENTATION_EXT: str = "VK_ERROR_FRAGMENTATION_EXT"; break;
        case VK_ERROR_FRAGMENTED_POOL: str = "VK_ERROR_FRAGMENTED_POOL"; break;
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: str = "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR"; break;
        case VK_ERROR_INCOMPATIBLE_DRIVER: str = "VK_ERROR_INCOMPATIBLE_DRIVER"; break;
        case VK_ERROR_INITIALIZATION_FAILED: str = "VK_ERROR_INITIALIZATION_FAILED"; break;
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: str = "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT"; break;
        // case VK_ERROR_INVALID_EXTERNAL_HANDLE: str = "VK_ERROR_INVALID_EXTERNAL_HANDLE"; break;
        case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR: str = "VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR"; break;
        case VK_ERROR_INVALID_SHADER_NV: str = "VK_ERROR_INVALID_SHADER_NV"; break;
        case VK_ERROR_LAYER_NOT_PRESENT: str = "VK_ERROR_LAYER_NOT_PRESENT"; break;
        case VK_ERROR_MEMORY_MAP_FAILED: str = "VK_ERROR_MEMORY_MAP_FAILED"; break;
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: str = "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR"; break;
        case VK_ERROR_NOT_PERMITTED_EXT: str = "VK_ERROR_NOT_PERMITTED_EXT"; break;
        case VK_ERROR_OUT_OF_DATE_KHR: str = "VK_ERROR_OUT_OF_DATE_KHR"; break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: str = "VK_ERROR_OUT_OF_DEVICE_MEMORY"; break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: str = "VK_ERROR_OUT_OF_HOST_MEMORY"; break;
        // case VK_ERROR_OUT_OF_POOL_MEMORY: str = "VK_ERROR_OUT_OF_POOL_MEMORY"; break;
        case VK_ERROR_OUT_OF_POOL_MEMORY_KHR: str = "VK_ERROR_OUT_OF_POOL_MEMORY_KHR"; break;
        case VK_ERROR_SURFACE_LOST_KHR: str = "VK_ERROR_SURFACE_LOST_KHR"; break;
        case VK_ERROR_TOO_MANY_OBJECTS: str = "VK_ERROR_TOO_MANY_OBJECTS"; break;
        case VK_ERROR_VALIDATION_FAILED_EXT: str = "VK_ERROR_VALIDATION_FAILED_EXT"; break;
        case VK_EVENT_RESET:
        case VK_EVENT_SET:
        case VK_INCOMPLETE:
        case VK_NOT_READY:
        case VK_SUBOPTIMAL_KHR:
        case VK_SUCCESS:
        case VK_TIMEOUT:
        default: str = "UNKNOWN"; break;
    }

    return str;
}