#include "core/application.hpp"
#include "core/renderer.hpp"

int main(int argc, char** argv) {
    std::cout << std::endl;
    log_info("starting octo...");

    application_config config{
        "octo", 1280, 720
    };


    if (!application_create_window(config)) {
        return -1;
    }

    if (!renderer_create_instance(application_get_window(), config)) {
        return -1;
    }

    if (!renderer_create_window_surface()) {
        return -1;
    }

    if (!renderer_create_device()) {
        return -1;
    }

    if (!renderer_create_swap_chain()) {
        return -1;
    }

    if (!renderer_create_render_pass()) {
        return -1;
    }

    if (!renderer_create_graphics_pipeline()) {
        return -1;
    }

    if (!renderer_create_command_pool()) {
        return -1;
    }

    if (!renderer_create_framebuffers()) {
        return -1;
    }

    application_start_main_loop();

    renderer_shutdown();
    application_shutdown();

    return 0;
}