#include "Renderer.h"
#include <SDL_image.h>
#include <iostream>

// 생성자: 창의 너비와 높이를 받아 SDL을 초기화하고 창과 렌더러를 생성합니다.
Renderer::Renderer(int width, int height) {
    // SDL 초기화
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }
    // 창 생성
    window = SDL_CreateWindow("SDL Layered Renderer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }
    // 렌더러 생성
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    // SDL_image 초기화
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
}

// 소멸자: 모든 레이어와 SDL 리소스를 해제합니다.
Renderer::~Renderer() {
    // 모든 레이어 해제
    for (auto layer : layers) {
        delete layer;
    }
    // SDL 리소스 해제
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    IMG_Quit();
    SDL_Quit();
}

// addLayer: 새로운 레이어를 추가합니다.
void Renderer::addLayer(const std::string& imagePath) {
    layers.push_back(new Layer(imagePath, renderer));
}

// render: 모든 레이어를 순서대로 화면에 그립니다.
void Renderer::render() {
    // 화면 초기화
    SDL_RenderClear(renderer);
    // 레이어 순서대로 그리기
    for (auto layer : layers) {
        layer->render(renderer);
    }
    // 화면 갱신
    SDL_RenderPresent(renderer);
}

// run: 메인 루프를 실행하여 이벤트를 처리하고 화면을 갱신합니다.
void Renderer::run() {
    bool running = true;
    SDL_Event event;

    while (running) {
        // 이벤트 처리
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        // 화면 그리기
        render();
        SDL_Delay(16); // 약 60 FPS
    }
}
