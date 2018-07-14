#include <exception>
#include <fstream>
#include <thread>

#include <SDL_image.h>
#include <SDL_main.h>

#include "engine/subspace.hpp"
#include "engine/video/sdl/sdl_window.hpp"
#include "engine/video/sdl/sdl_renderer.hpp"

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
		string basepath = SDL_GetBasePath();
		string filename = basepath.substr(0, basepath.rfind("subspace")) + "subspace/rc/logo.png";
		SDL_Surface* image = IMG_Load(filename.c_str());
		if (image == nullptr) {
			logger.logWarning("Failed to load image - SDL_Image error: {}", IMG_GetError());
		}
		sprite.texture = SDL_CreateTextureFromSurface(renderer, image);

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
    } catch (const exception& e) {
        logger.logError(e.what());
    }

    return 0;
}
