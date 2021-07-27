#include "core/application.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <spdlog/spdlog.h>

int main(int argc, char **argv)
{
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("ProjectName: {}", PROJECT_NAME);

    for (int i = 0; i < argc; i++)
    {
        spdlog::debug("args: {}: {}", i, argv[i]);
    }

    octo::Application app{argc, argv};

    try
    {
        if (app.init()) {
            app.start();
        }
    }
    catch (const std::exception &e)
    {
        spdlog::error(e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}