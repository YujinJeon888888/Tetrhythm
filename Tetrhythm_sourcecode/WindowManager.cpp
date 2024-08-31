#include "WindowManager.h"

WindowManager::WindowManager(const char* title, int width, int height) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init(); // 폰트출력추가
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == nullptr) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit(); // 폰트출력추가
        SDL_Quit();
        exit(1);
    }

    // [추가된 부분 시작] 기본 배경색을 검은색으로 설정
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // R, G, B, A 값 (0, 0, 0, 255 = 검은색)
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    // [추가된 부분 끝]
}

WindowManager::~WindowManager() {
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != nullptr) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

SDL_Renderer* WindowManager::getRenderer() const {
    return renderer;
}

void WindowManager::clear() {
    // [추가된 부분 시작] 기본 배경색을 검은색으로 설정
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // R, G, B, A 값 (0, 0, 0, 255 = 검은색)
    SDL_RenderClear(renderer);
    // [추가된 부분 끝]
}

void WindowManager::present() {
    SDL_RenderPresent(renderer);
}

SDL_Window* WindowManager::getWindow() const {
    return window;
}

