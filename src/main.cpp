#include "core/context.hpp"
#include "core/window.hpp"
#include "core/renderer.hpp"
#include "utils/logger.hpp"

int main(int argc, char** argv) {
    LogDebug("main: starting octo...");

    octo::Context context{};
    octo::Window window{&context};
    octo::Renderer renderer{&context};

    LogDebug("main: setting up event listeners");
    window.onWindowResized([&](int width, int height) {
        renderer.resize(width, height);
    });

    window.onKeyPressed(GLFW_KEY_ESCAPE, [&](int mods) {
        LogDebug("main: onKeyPress: GLFW_KEY_ESCAPE");
        window.shutdown();
    });

    LogDebug("main: Starting MainLoop");
    while (window.pollEvents()) {
        renderer.render();
    }

    return 0;
}