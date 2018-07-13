#include <exception>
#include <fstream>
#include <thread>

#include "engine/subspace.hpp"
#include "engine/video/vulkan/vulkan_renderer.hpp"
#include "engine/video/vulkan/vulkan_window.hpp"

#undef main

using namespace std;
using namespace subspace;

int main(int, char**) {
    fstream logFile("log.txt", ios::out | ios::trunc);
    logger.setOutput(logFile);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    try {
        logger.logInfo("Starting Subspace Engine...");
        Config config {"rc/config.json"};

        VulkanRenderer renderer;
        VulkanWindow window(renderer, "Subspace", config);

        bool running = true;
        SDL_Event event;

        while (running) {
            // TODO: Add event handling system
            while (SDL_PollEvent(&event)) {
                switch(event.type) {
                    case SDL_KEYDOWN:
                        switch(event.key.keysym.sym) {
                            case SDLK_ESCAPE:
                                running = false;
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }

            window.swap();
        }
    } catch (const exception& e) {
        logger.logError(e.what());
    }

    return 0;
}
