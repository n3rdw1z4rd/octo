#include <octo.h>

int main(int argc, char **argv)
{
    LOG_INFO("*** OCTO TEST ***");
    LOG_DEBUG("DEBUG BUILD");

    // LogMessage(LOG_LEVEL_FATAL, "octotest: LogFatal");

    // LogFatal("octotest: LogFatal");
    // LogError("octotest: LogError");
    // LogWarn("octotest: LogWarn");
    // LogInfo("octotest: LogInfo");
    // LogDebug("octotest: LogDebug");
    // LogTrace("octotest: LogTrace");

    // Context context{};
    // context.name = "octo | test";

    // Window window{&context};
    // Renderer renderer{&context};

    // LogDebug("octotest: setting up event listeners");

    // window.onWindowResized([&](int width, int height)
    //                        { renderer.resize(width, height); });

    // window.onKeyPressed(GLFW_KEY_ESCAPE, [&](int mods)
    //                     {
    //     LogDebug("octotest: onKeyPress: GLFW_KEY_ESCAPE");
    //     window.shutdown(); });

    // LogDebug("octotest: starting main loop");

    // while (window.pollEvents())
    // {
    //     // renderer.render();
    // }

    return 0;
}
