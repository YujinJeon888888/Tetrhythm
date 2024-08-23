#pragma once
#include "Game.h"
#include <iostream>
#include "ClearScene.h"
#include "SceneManager.h"
#include "GameOverScene.h"
#include "MainMenu.h"
#include "MySQL.h"
#include "UserInfo.h"
#include <vector>
#include <string>

class TetrisScene : public Scene {

public:
    TetrisScene(WindowManager& wm, SceneManager& manager);
    ~TetrisScene();

    void drawInit() override;
    void handleEvents() override;
    void update() override;
    void render() override;

private:
    Print* print;
    WindowManager& windowManager;
    SceneManager& sceneManager;
    Game* game;
};
