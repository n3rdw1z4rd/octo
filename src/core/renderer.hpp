#pragma once

#include "defines.hpp"
#include "types.hpp"
#include "logger.hpp"

#include <GLFW/glfw3.h>

bool renderer_create_instance(GLFWwindow* window, application_config config);
bool renderer_create_window_surface();
bool renderer_create_device();
bool renderer_create_swap_chain();

void renderer_shutdown();