#include "Layer.h"
#include <SDL_image.h>
#include <iostream>

Layer::Layer(const std::string& imagePath, SDL_Renderer* renderer, int x, int y, int zIndex, LayerType type)
    : zIndex(zIndex), type(type) {
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return;
    }
    dstRect.x = x;
    dstRect.y = y;
    SDL_QueryTexture(texture, nullptr, nullptr, &dstRect.w, &dstRect.h);
}

Layer::~Layer() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Layer::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    }
}

int Layer::getZIndex() const {
    return zIndex;
}

LayerType Layer::getType() const {
    return type;
}
