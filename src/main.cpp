#include "core/application_desc.hpp"
#include "core/window.hpp"
#include "core/renderer.hpp"
#include "utils/logger.hpp"

int main(int argc, char** argv) {
    octo::ApplicationDesc appInfo{
        .name = "octo"
    };

    octo::Window window{&appInfo};
    octo::Renderer renderer{&appInfo};

    window.onKeyPressed(GLFW_KEY_ESCAPE, [&](int mods) {
        window.shutdown();
    });

    while (window.pollEvents()) {
        renderer.render();
    }

    return 0;
}