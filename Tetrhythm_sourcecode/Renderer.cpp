#include "Renderer.h"
#include <SDL_image.h>
#include <iostream>
#include <algorithm>

Renderer::Renderer(int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }
    window = SDL_CreateWindow("SDL Layered Renderer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
}

Renderer::~Renderer() {
    for (auto layer : layers) {
        delete layer;
    }
    for (auto animation : animations) {
        delete animation;
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    IMG_Quit();
    SDL_Quit();
}

void Renderer::addLayer(const std::string& imagePath, int x, int y, int zIndex, LayerType type) {
    layers.push_back(new Layer(imagePath, renderer, x, y, zIndex, type));
    std::sort(layers.begin(), layers.end(), [](Layer* a, Layer* b) {
        if (a->getType() == b->getType()) {
            return a->getZIndex() < b->getZIndex();
        }
        return a->getType() < b->getType();
        });
}

void Renderer::addAnimation(const std::vector<std::string>& imagePaths, int x, int y, int zIndex, LayerType type, int frameDelay) {
    animations.push_back(new Animation(imagePaths, renderer, x, y, zIndex, type, frameDelay));
    std::sort(animations.begin(), animations.end(), [](Animation* a, Animation* b) {
        if (a->getType() == b->getType()) {
            return a->getZIndex() < b->getZIndex();
        }
        return a->getType() < b->getType();
        });
}

void Renderer::render() {
    SDL_RenderClear(renderer);
    for (auto layer : layers) {
        layer->render(renderer);
    }
    for (auto animation : animations) {
        animation->render(renderer);
    }
    SDL_RenderPresent(renderer);
}

// ���� ���� ����: �������� �����ϰ�, �̺�Ʈ�� ó���ϰ�, ȭ���� �����մϴ�.
void Renderer::run() {
    bool running = true; // ���� ���� ����
    SDL_Event event; // SDL �̺�Ʈ ����ü
    Uint32 frameStart; // ���� �������� ���۵� �ð�
    int frameTime; // ���� �������� ó���ϴ� �� �ɸ� �ð�

    // ���� ����
    while (running) {
        frameStart = SDL_GetTicks(); // ���� �������� ���۵� �ð� ���

        // �̺�Ʈ ó�� ����
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false; // â �ݱ� �̺�Ʈ �߻� �� ���� ����
            }
        }

        render(); // ȭ�� ����

        frameTime = SDL_GetTicks() - frameStart; // ���� �������� ó���ϴ� �� �ɸ� �ð� ���

        // ��ǥ ������ �ӵ��� �����ϱ� ���� ���� �ð���ŭ ���
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime); // ������ �����̰� ���� �ִ� ��� ���
        }
    }
}
