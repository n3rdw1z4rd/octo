#include <octo.hpp>

int main(int argc, char **argv)
{
    LogDebug("main: starting", ENGINE_NAME, OCTO_VERSION);
    LogDebug("main: assets path:", octo::OCTO_ASSETS_PATH + octo::TEXTURE_PATH);

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

    while (window.pollEvents())
    {
        renderer.render();
    }

    return 0;
}