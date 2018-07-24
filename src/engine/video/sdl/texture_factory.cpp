#include "texture_factory.hpp"

// Project includes
#include "sdl_renderer.hpp"
#include "engine/util/logger.hpp"

using namespace subspace;

TextureFactory::TextureFactory(const SdlRenderer& renderer) :
    renderer_(renderer)
{
}

TextureFactory::Handle::Handle() :
    handle_(nullptr)
{
}

TextureFactory::Handle::~Handle() {
    if (handle_ != nullptr) {
        handle_->referenceCount--;
    }
}

TextureFactory::Handle::Handle(Texture& handle) :
    handle_(&handle)
{
    handle_->referenceCount++;
}

TextureFactory::Handle& TextureFactory::Handle::operator=(const Handle& rhs) {
    if (handle_ != nullptr) {
        handle_->referenceCount--;
    }

    handle_ = rhs.handle_;
    handle_->referenceCount++;

    return *this;
}

TextureFactory::Texture& TextureFactory::Handle::operator*() {
    return *handle_;
}

TextureFactory::Handle TextureFactory::getTexture(const std::string& name) {
    auto result = textures_.find(name);

    if (result == textures_.end()) {
        Texture* texture = loadTexture(name);
        
        if (texture == nullptr) {
            logger.logWarning("Unable to create a handle for texture {}", name);
            return Handle();
        } else {
            return Handle(*texture);
        }
    } else {
        return Handle(result->second);
    }
}

void TextureFactory::checkResources() {
    for (auto i = textures_.begin(); i != textures_.end();) {
        if (i->second.referenceCount <= 0) {
            SDL_DestroyTexture(i->second.handle);
            i = textures_.erase(i);
        } else {
            i++;
        }
    }
}

TextureFactory::Texture* TextureFactory::loadTexture(const std::string& name) {
    std::string basepath = SDL_GetBasePath();
    std::string filename = basepath.substr(0, basepath.rfind("subspace")) + "subspace/" + name;

    SDL_Surface* image = IMG_Load(filename.c_str());
    if (image == nullptr) {
        logger.logWarning("Failed to load image - SDL_Image error: {}", IMG_GetError());
        return nullptr;
    }

    SDL_Texture* textureRaw = SDL_CreateTextureFromSurface(renderer_, image);
    if (textureRaw == nullptr) {
        logger.logWarning("Failed to create texture - SDL error: {}", SDL_GetError());
        SDL_FreeSurface(image);
        return nullptr;
    }

    Texture texture {name, {image->w, image->h}, 0, textureRaw};

    auto result = textures_.insert({name, texture});
    if (!result.second) {
        logger.logWarning("Could not insert new texture into map");
        SDL_FreeSurface(image);
        SDL_DestroyTexture(textureRaw);
        return nullptr;
    }

    SDL_FreeSurface(image);
    return &((*result.first).second);
}
