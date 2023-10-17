#include <octo.hpp>
#include <renderer/renderer.hpp>

int main(int argc, char **argv)
{
    octo::Context context{};
    context.name = "octo | test";

    octo::Window window{&context};
    octo::Renderer renderer{&context};

    octo::LogDebug("octotest: setting up event listeners");

    window.onWindowResized([&](int width, int height)
                           { renderer.resize(width, height); });

    window.onKeyPressed(GLFW_KEY_ESCAPE, [&](int mods)
                        {
        octo::LogDebug("octotest: onKeyPress: GLFW_KEY_ESCAPE");
        window.shutdown(); });

    octo::LogDebug("octotest: starting main loop");

    while (window.pollEvents())
    {
        // renderer.render();
    }

    return 0;
}
