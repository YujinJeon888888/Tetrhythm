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

// 게임 루프 실행: 프레임을 관리하고, 이벤트를 처리하고, 화면을 갱신합니다.
void Renderer::run() {
    bool running = true; // 루프 제어 변수
    SDL_Event event; // SDL 이벤트 구조체
    Uint32 frameStart; // 현재 프레임이 시작된 시간
    int frameTime; // 현재 프레임을 처리하는 데 걸린 시간

    // 게임 루프
    while (running) {
        frameStart = SDL_GetTicks(); // 현재 프레임이 시작된 시간 기록

        // 이벤트 처리 루프
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false; // 창 닫기 이벤트 발생 시 루프 종료
            }
        }

        render(); // 화면 갱신

        frameTime = SDL_GetTicks() - frameStart; // 현재 프레임을 처리하는 데 걸린 시간 계산

        // 목표 프레임 속도를 유지하기 위해 남은 시간만큼 대기
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime); // 프레임 딜레이가 남아 있는 경우 대기
        }
    }
}
