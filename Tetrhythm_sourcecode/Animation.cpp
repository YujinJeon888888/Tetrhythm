#include "Animation.h"
#include <SDL_image.h>
#include <iostream>

Animation::Animation(const std::vector<std::string>& imagePaths, SDL_Renderer* renderer, int x, int y, int zIndex, LayerType type, int frameDelay)
    : zIndex(zIndex), type(type), frameDelay(frameDelay), currentFrame(0), lastFrameTime(0) {
    for (const auto& path : imagePaths) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
            continue;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
            continue;
        }
        textures.push_back(texture);
    }
    if (!textures.empty()) {
        dstRect.x = x;
        dstRect.y = y;
        SDL_QueryTexture(textures[0], nullptr, nullptr, &dstRect.w, &dstRect.h);
    }
}

Animation::~Animation() {
    for (auto texture : textures) {
        SDL_DestroyTexture(texture);
    }
}

void Animation::render(SDL_Renderer* renderer) {
    if (textures.empty()) return;

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastFrameTime > static_cast<Uint32>(frameDelay)) {
        currentFrame = (currentFrame + 1) % textures.size();
        lastFrameTime = currentTime;
    }

    SDL_RenderCopy(renderer, textures[currentFrame], nullptr, &dstRect);
}

int Animation::getZIndex() const {
    return zIndex;
}

LayerType Animation::getType() const {
    return type;
}
