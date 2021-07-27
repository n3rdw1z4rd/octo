#pragma once

#include "defines.hpp"

#include <vulkan/vulkan.h>

namespace octo
{
    class Renderer
    {
        VkInstance instance;

    public:
        Renderer();
        ~Renderer();

        void init();
    };
}