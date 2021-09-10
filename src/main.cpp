#include "core/application.hpp"
#include "renderer/renderer.hpp"

#define TITLE "octo demo"
#define WIDTH 1280
#define HEIGHT 720

int main(int argc, char** argv) {
    Application app{};
    Renderer renderer{};
    
    if (
        app.initialize(TITLE, WIDTH, HEIGHT) &&
        renderer.initialize(TITLE, app.getWindowHandle())
    ) {
        app.run([&renderer](bool resized) {
            renderer.render(resized);
        });

        // renderer.cleanup();
    }

    return 0;
}