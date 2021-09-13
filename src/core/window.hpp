#pragma once

#include "defines.hpp"

#include <GLFW/glfw3.h>
#include <sstream>
#include <string>
#include <memory>
#include <functional>
#include <list>
#include <map>

namespace octo {
    struct WindowInfo {
        std::string name = "octo";
        uint32_t width = 1280;
        uint32_t height = 720;
        bool fullscreen = false;
        GLFWwindow* windowHandle;

        std::string toString() {
            std::stringstream ss;

            ss << "{ ";
            ss << "name: " << name;
            ss << ", width: " << width;
            ss << ", height: " << height;
            ss << ", fullscreen: " << fullscreen ? "true" : "false";
            ss << ", windowHandle: " << windowHandle;
            ss << " }";

            return ss.str();
        }
    };

    struct MousePosition {
        int x;
        int y;
    };

    struct KeyboardState {
        bool keyDown[GLFW_KEY_LAST + 1] = { false };
        long long keyDownTime[GLFW_KEY_LAST + 1] = { 0 };
        int mods;
    };

    struct MouseState {
        bool buttonDown[GLFW_MOUSE_BUTTON_LAST + 1] = { false };
        long long buttonDownTime[GLFW_MOUSE_BUTTON_LAST + 1] = { 0 };
        int mods;
        int wheel;
        MousePosition position;
        MousePosition positionDelta;
    };

    class Window {
    public:
        Window();
        ~Window();

        bool create(WindowInfo*);
        GLFWwindow* getWindow() { return _windowHandle; }
        bool pollEvents();
        void shutdown();
        long long getTimestamp();

        void onKeyboardEvent(std::function<void(int key, int scancode, int action, int mods)> callback);
        void onKeyPressed(int key, std::function<void(int mods)> callback);

        void onMouseButtonEvent(std::function<void(int button, int action, int mods)> callback);
        void onButtonClicked(int button, std::function<void(int mods)> callback);

        bool isKeyDown(uint32_t key);
        bool isMouseButtonDown(uint32_t button);

        void onWindowResized(std::function<void(int width, int height)> callback);

    private:
        static void _keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void _mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void _mouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void _mousePositionCallback(GLFWwindow* window, double xpos, double ypos);
        static void _resizeCallback(GLFWwindow* window, int width, int height);

        bool _windowCreated = false;

        std::string _title;
        uint32_t _width;
        uint32_t _height;

        GLFWwindow* _windowHandle;

        int _downTimeThreshold = 250;

        KeyboardState _keyboardState;
        std::list<std::function<void(int key, int scancode, int action, int mods)>> _keyboardEventListeners;
        std::multimap<int, std::function<void(int mods)>> _keyPressedEventListeners;

        MouseState _mouseState;
        std::list<std::function<void(int button, int action, int mods)>> _mouseButtonEventListeners;
        std::multimap<int, std::function<void(int mods)>> _buttonClickedEventListeners;

        std::list<std::function<void(int width, int height)>> _windowResizedListeners;
    };

    // using WindowPtr = std::shared_ptr<Window>;
}