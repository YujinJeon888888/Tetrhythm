#pragma once
#include <SDL.h>
#include <iostream>
#include <SDL_ttf.h> // 폰트출력추가
class WindowManager {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    WindowManager(const char* title, int width, int height);
    ~WindowManager();

    SDL_Renderer* getRenderer() const;
    SDL_Window* getWindow() const;
    void clear();
    void present();
};
