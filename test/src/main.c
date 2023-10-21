#include <octo.h>

// bool _on_game_event(game_context *context, event_type type, void *data)
// {
//     switch (type)
//     {
//     case EVENT_TYPE_SHUTDOWN:
//         context->is_running = false;
//         break;
//     case EVENT_TYPE_KEY_RELEASED:
//         LOG_DEBUG("- key released: %i", data);
//         break;
//     default:
//         break;
//     }

//     return false;
// }

int main(int argc, char **argv)
{
    LOG_INFO("*** OCTO TEST ***");

    window *window = create_window("octo - test", 1280, 720);

    if (window != NULL)
    {
        while (!window->should_close)
        {
            poll_window_events();

            // mouse_position mpos = get_mouse_postion();
            // LOG_DEBUG("mpos: %ix%i", mpos.x, mpos.y);

            if (is_key_down(GLFW_KEY_ESCAPE))
            {
                break;
            }
        }

        destroy_window();

        return 0;
    }

    return -1;
}
