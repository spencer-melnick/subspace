#pragma once

// STL includes
#include <string>
#include <unordered_map>

// Library includes
#include <SDL_render.h>
#include <SDL_image.h>

// Project includes
#include "engine/math/vec2.hpp"

namespace subspace {

    class SdlRenderer;

    class TextureFactory {
        private:
            // Forward declare Texture for Handle
            struct Texture;

        public:
            TextureFactory(const SdlRenderer& renderer);

            // Nested handle class
            class Handle {
                public:
                    Handle();
                    ~Handle();

                    Handle& operator=(const Handle& rhs);
                    Texture& operator*();

                private:
                    friend class TextureFactory;
                    
                    Handle(Texture& handle);

                    Texture* handle_;
            };

            // Member functions
            Handle getTexture(const std::string& name);
            void checkResources();

        private:
            // Texture struct
            struct Texture {
                std::string name;
                Vec2i dimension;
                unsigned referenceCount;
                SDL_Texture* handle;
            };

            // Helper functions
            Texture* loadTexture(const std::string& name);

            // Private members
            const SdlRenderer& renderer_;
            std::unordered_map<std::string, Texture> textures_;
    };
}
