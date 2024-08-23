#pragma once

#include "Print.h"
#include "WindowManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include <SDL.h>
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <SDL_image.h>
#include <string>


class MainMenu : public Scene {
public:
    MainMenu(WindowManager& wm, SceneManager& manager);
    // 오버라이드된 메서드
    void drawInit() override;
    void handleEvents() override;
    void update() override;
    void render() override;

private:
    void drawSelection();
    void handleArrowKey(SDL_Keycode key);

    static const int numOptions = 3;
    int MenuSelection = 0;
    Print* print;
    WindowManager& windowManager;
    SceneManager& sceneManager;
    SoundManager * soundManager;  // SoundManager 객체 추가
};
