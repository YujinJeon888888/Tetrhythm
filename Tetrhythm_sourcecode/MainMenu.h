#pragma once

#include "Print.h"
#include "WindowManager.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "Scene.h"
#include "SceneManager.h"

class MainMenu : public Scene {
public:
    MainMenu(WindowManager& wm, SceneManager& manager);
    void drawInit() override;
    void drawSelection();
    void handleArrowKey(SDL_Keycode key);
    void handleEvents() override;
    void update() override;
    void render() override;

private:
    static const int numOptions = 3;
    int MenuSelection = 0;
    Print* print;
    WindowManager& windowManager;
    SceneManager& sceneManager;
};