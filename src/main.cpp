#include "core/window.hpp"
#include "core/renderer.hpp"
#include "utils/logger.hpp"

int main(int argc, char** argv) {
    octo::Window window{};
    octo::Renderer renderer{};

    octo::WindowInfo wInfo{
        .name = "octo"
    };

    if (!window.create(&wInfo)) {
        LogError("failed to create widow");
        return -1;
    }

    renderer.create(&wInfo);

    window.onKeyPressed(GLFW_KEY_ESCAPE, [&](int mods) {
        window.shutdown();
    });

    // window.onWindowResized([&](int width, int height) {
    //     renderer.resize(width, height);
    // });

    while (window.pollEvents()) {
        renderer.render();
    }

    return 0;
}