#include "window.hpp"
#include "utils/logger.hpp"

#include <GLFW/glfw3native.h>
#include <chrono>

namespace octo {
    Window::Window(Context* context) {
        LogDebug("Window: initializing...");

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        _context = context;

        _context->windowHandle = glfwCreateWindow(_context->width, _context->height, _context->name.c_str(), nullptr, nullptr);

        if (!_context->windowHandle) {
            throw std::runtime_error("Window: failed to create window");
        }

        glfwSetWindowUserPointer(_context->windowHandle, this);

        glfwSetKeyCallback(_context->windowHandle, &Window::_keyCallback);
        glfwSetMouseButtonCallback(_context->windowHandle, &Window::_mouseButtonCallback);
        glfwSetScrollCallback(_context->windowHandle, &Window::_mouseWheelCallback);
        glfwSetCursorPosCallback(_context->windowHandle, &Window::_mousePositionCallback);
        glfwSetFramebufferSizeCallback(_context->windowHandle, &Window::_resizeCallback);
    }

    Window::~Window() {
        LogDebug("Window::~Window");

        glfwDestroyWindow(_context->windowHandle);
        glfwTerminate();
    }

    bool Window::pollEvents() {
        if (_context->windowHandle) {
            glfwPollEvents();
            return !glfwWindowShouldClose(_context->windowHandle);
        } else {
            LogWarn("Window::pollEvents: window does not exist");
            return false;
        }
    }

    void Window::shutdown() {
        glfwSetWindowShouldClose(_context->windowHandle, 1);
    }

    long long Window::getTimestamp() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }

    void Window::_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));

        windowPtr->_keyboardState.mods = mods;

        if (action == GLFW_PRESS) {
            windowPtr->_keyboardState.keyDown[key] = true;
            windowPtr->_keyboardState.keyDownTime[key] = windowPtr->getTimestamp();
        } else {
            windowPtr->_keyboardState.keyDown[key] = false;

            if (windowPtr->getTimestamp() - windowPtr->_keyboardState.keyDownTime[key] < windowPtr->_context->inputDownTimeThreshold) {
                for (auto callback : windowPtr->_keyPressedEventListeners) {
                    if (callback.first == key) {
                        callback.second(mods);
                    }
                }
            }

            windowPtr->_keyboardState.keyDownTime[key] = 0;
        }

        for (auto callback : windowPtr->_keyboardEventListeners) {
            callback(key, scancode, action, mods);
        }
    }

    void Window::_mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        auto windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));

        windowPtr->_mouseState.mods = mods;

        if (action == GLFW_PRESS) {
            windowPtr->_mouseState.buttonDown[button] = true;
            windowPtr->_mouseState.buttonDownTime[button] = windowPtr->getTimestamp();
        } else {
            windowPtr->_mouseState.buttonDown[button] = false;

            if (windowPtr->getTimestamp() - windowPtr->_mouseState.buttonDownTime[button] < windowPtr->_context->inputDownTimeThreshold) {
                for (auto callback : windowPtr->_buttonClickedEventListeners) {
                    if (callback.first == button) {
                        callback.second(mods);
                    }
                }
            }
        }

        for (auto callback : windowPtr->_mouseButtonEventListeners) {
            callback(button, action, mods);
        }
    }

    void Window::_mouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset) {
        auto windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
    }

    void Window::_mousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
        auto windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));
    }

    void Window::onKeyboardEvent(std::function<void(int key, int scancode, int action, int mods)> callback) {
        _keyboardEventListeners.push_back(callback);
    }

    void Window::onKeyPressed(int key, std::function<void(int mods)> callback) {
        _keyPressedEventListeners.insert({ key, callback });
    }

    void Window::onMouseButtonEvent(std::function<void(int button, int action, int mods)> callback) {
        _mouseButtonEventListeners.push_back(callback);
    }

    void Window::onButtonClicked(int button, std::function<void(int mods)> callback) {
        _buttonClickedEventListeners.insert({ button, callback });
    }

    void Window::_resizeCallback(GLFWwindow* window, int width, int height) {
        auto windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));

        for (auto callback : windowPtr->_windowResizedListeners) {
            callback(width, height);
        }
    }

    bool Window::isKeyDown(uint32_t key) {
        return (key < GLFW_KEY_LAST) ? _keyboardState.keyDown[key] : false;
    }

    bool Window::isMouseButtonDown(uint32_t button) {
        return (button < GLFW_MOUSE_BUTTON_LAST) ? _mouseState.buttonDown[button] : false;
    }

    void Window::onWindowResized(std::function<void(int width, int height)> callback) {
        _windowResizedListeners.push_back(callback);
    }

}