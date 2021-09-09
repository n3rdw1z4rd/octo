#pragma once

#include "defines.hpp"
#include "types.hpp"
#include "logger.hpp"

#include <GLFW/glfw3.h>
#include <vector>
#include <string>

bool application_create_window(application_config app_config);
void application_start_main_loop();
void application_shutdown();
GLFWwindow* application_get_window();