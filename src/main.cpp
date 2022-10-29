#include "../include/octo.hpp"

int main(int argc, char** argv) {
    // octo_init();

    // auto *handle = octo_create_default_window();

    // octo_main_loop(handle);
    // octo_shutdown(handle);

    log_debug("debug");
    log_info("info");
    log_system("system");
    log_warn("warn");
    log_error("error");

    return 0;
}