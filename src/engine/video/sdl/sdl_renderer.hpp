#pragma once

// STL includes
#include <string>
#include <vector>
#include <memory>

// Library includes
#include <SDL_render.h>

// Project includes
#include "engine/video/renderer.hpp"
#include "sdl_window.hpp"
#include "texture_factory.hpp"

namespace subspace {
	struct SdlSprite {
		Vec2 position, scale {1.0, 1.0};
		Vec2i source;
		TextureFactory::Handle texture;
	};

	class SdlRenderer: public IRenderer {
		public:
			SdlRenderer(const SdlWindow& window);
			~SdlRenderer();

			// Cast to wrapped type
			operator SDL_Renderer*() const;

			// Interface functions
			virtual void draw() override;

			// Member functions
			SdlSprite& createSprite();
			void destroySprite(SdlSprite& sprite);
			TextureFactory& getTextureFactory();

		private:
			const SdlWindow& window_;
			SDL_Renderer* handle_;
			std::unique_ptr<TextureFactory> textureFactory_;
			std::vector<SdlSprite> sprites_;
	};
}
