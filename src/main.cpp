#include <exception>
#include <fstream>
#include <thread>

#include <SDL_image.h>
#include <SDL_main.h>

#include "engine/subspace.hpp"
#include "engine/video/sdl/sdl_window.hpp"
#include "engine/video/sdl/sdl_renderer.hpp"
#include "engine/video/sdl/texture_factory.hpp"

using namespace std;
using namespace subspace;

int main(int, char**) {
    fstream logFile("log.txt", ios::out | ios::trunc);
    logger.setOutput(logFile);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	IMG_Init(IMG_INIT_PNG);

    try {
        logger.logInfo("Starting Subspace Engine...");
        Config config {"rc/config.json"};

        SdlWindow window("Subspace", config);
		SdlRenderer renderer(window);

        bool running = true;
        SDL_Event event;

		auto& sprite = renderer.createSprite();
		sprite.texture = renderer.getTextureFactory().getTexture("rc/logo.png");

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
			renderer.draw();
        }

        renderer.destroySprite(sprite);
        renderer.getTextureFactory().checkResources();
    } catch (const exception& e) {
        logger.logError(e.what());
    }

    return 0;
}
