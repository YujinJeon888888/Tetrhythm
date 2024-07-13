#pragma once
#include <SDL.h>
#include <iostream>

class WindowManager {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    WindowManager(const char* title, int width, int height);
    ~WindowManager();

    SDL_Renderer* getRenderer() const;
    void clear();
    void present();
};
