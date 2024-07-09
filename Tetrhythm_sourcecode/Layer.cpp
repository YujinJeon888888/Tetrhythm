#include "Layer.h"
#include <SDL_image.h>
#include <iostream>

// 생성자: 이미지 파일 경로와 SDL_Renderer를 받아 이미지를 로드하고 텍스처를 생성합니다.
Layer::Layer(const std::string& imagePath, SDL_Renderer* renderer) {
    // PNG 이미지 로드
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }
    // SDL_Surface를 SDL_Texture로 변환
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    }
}

// 소멸자: 텍스처를 해제합니다.
Layer::~Layer() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

// render: SDL_Renderer에 텍스처를 그립니다.
void Layer::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    }
}
