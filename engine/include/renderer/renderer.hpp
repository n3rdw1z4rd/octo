#include "../core/context.hpp"

namespace octo
{
    class Renderer
    {
    public:
        Renderer(Context *context);
        ~Renderer();

        void render();
        void resize(int width, int height);

    private:
        Context *context;
        VkInstance instance;

        void initVulkan();
        void cleanup();
        void createInstance();
    };
}