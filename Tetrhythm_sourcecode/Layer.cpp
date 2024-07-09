#include "Layer.h"
#include <SDL_image.h>
#include <iostream>

// ������: �̹��� ���� ��ο� SDL_Renderer�� �޾� �̹����� �ε��ϰ� �ؽ�ó�� �����մϴ�.
Layer::Layer(const std::string& imagePath, SDL_Renderer* renderer) {
    // PNG �̹��� �ε�
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }
    // SDL_Surface�� SDL_Texture�� ��ȯ
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    }
}

// �Ҹ���: �ؽ�ó�� �����մϴ�.
Layer::~Layer() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

// render: SDL_Renderer�� �ؽ�ó�� �׸��ϴ�.
void Layer::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    }
}
