#include "WindowManager.h"

WindowManager::WindowManager(const char* title, int width, int height) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == nullptr) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
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
    SDL_RenderClear(renderer);
}

void WindowManager::present() {
    SDL_RenderPresent(renderer);
}

SDL_Window* WindowManager::getWindow() const {
    return window;
}

