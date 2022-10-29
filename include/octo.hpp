#pragma once

#include "defines.hpp"
#include "logger.hpp"
#include "dictionary.hpp"

#include <GLFW/glfw3.h>

bool octo_init();
GLFWwindow *octo_create_window(unsigned int, unsigned int, const char *, bool);
GLFWwindow *octo_create_default_window();
void octo_main_loop(GLFWwindow *);
void octo_shutdown(GLFWwindow *);