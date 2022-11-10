#include <octo.hpp>

int main(int argc, char **argv)
{
    octo::Context *ctx = octo::create_default_context();

    if (!octo::init(ctx)) {
        return 1;
    }

    octo::main_loop(ctx);
    octo::shutdown(ctx);

    return 0;
}