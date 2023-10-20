#include <octo.h>

int main(int argc, char **argv)
{
    LOG_INFO("*** OCTO TEST ***");

    GameContext *ctx = create_game_context("octo | test");

    if (game_init(ctx))
    {
        while (true)
        {
            game_poll_events();
        }

        // Window window{&ctx};
        // Renderer renderer{&ctx};

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
    }

    game_shutdown(ctx);

    return 0;
}
