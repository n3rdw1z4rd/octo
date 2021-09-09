#include "application.hpp"

static struct application_state {
    GLFWwindow* window;
    bool is_running = false;
    bool initialized = false;
    bool frame_buffer_resized = false;
} state;

static void _framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
    log_debug("_framebuffer_resize_callback: width: ", width, ", height: ", height);
    state.frame_buffer_resized = true;
}

void _key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    log_debug("_key_callback: key: ", key, ", scancode: ", scancode, ", action: ", action, ", mods: ", mods);
}

void _mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    log_debug("_mouse_button_callback: button: ", button, ", action: ", action, ", mods: ", mods);
}

void _mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset) {
    log_debug("_mouse_wheel_callback: xoffset: ", xoffset, ", yoffset: ", yoffset);
}

void _mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {
    // log_debug("_mouse_position_callback: xpos: ", xpos, ", ypos: ", ypos);
}

bool application_create_window(application_config app_config) {
    log_debug("application_create_window");

    if (!state.initialized) {
        if (glfwInit() == GLFW_FALSE) {
            log_error("application_create_window: failed to initialize glfw");
            return false;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        state.window = glfwCreateWindow(
            app_config.window_width,
            app_config.window_height,
            app_config.window_title.c_str(),
            nullptr, nullptr
        );

        if (!state.window) {
            log_error("application_create_window: failed to create window");
            return false;
        }

        // glfwSetWindowUserPointer(state.window, ???);
        glfwSetFramebufferSizeCallback(state.window, _framebuffer_resize_callback);
        glfwSetKeyCallback(state.window, &_key_callback);
        glfwSetMouseButtonCallback(state.window, &_mouse_button_callback);
        glfwSetScrollCallback(state.window, &_mouse_wheel_callback);
        glfwSetCursorPosCallback(state.window, &_mouse_position_callback);

        state.initialized = true;
    } else {
        log_warn("application_create_window: already initialized");
    }

    return true;
}

void application_start_main_loop() {
    log_debug("application_start_main_loop");

    if (state.window) {
        if (!state.is_running) {
            while (!glfwWindowShouldClose(state.window)) {
                glfwPollEvents();
            }
        } else {
            log_warn("application_start_main_loop: already running");
        }
    } else {
        log_error("application_start_main_loop: must create application window first");
    }
}

void application_shutdown() {
    log_debug("application_shutdown");

    if (state.window) {
        glfwDestroyWindow(state.window);
    }

    glfwTerminate();
}

GLFWwindow* application_get_window() {
    return state.window;
}