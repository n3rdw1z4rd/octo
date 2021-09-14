#include "core/context.hpp"
#include "core/window.hpp"
#include "core/renderer.hpp"
#include "utils/logger.hpp"

int main(int argc, char** argv) {
    octo::Context context{};
    octo::Window window{&context};
    octo::Renderer renderer{&context};

    window.onKeyPressed(GLFW_KEY_ESCAPE, [&](int mods) {
        window.shutdown();
    });

    while (window.pollEvents()) {
        renderer.render();
    }

    return 0;
}