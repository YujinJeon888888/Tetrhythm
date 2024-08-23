#pragma once
#include <iostream>
#include "ClearScene.h"
#include "SceneManager.h"
#include "MultiGame.h"
#include "MultiGameScene.h"
#include "GameOverScene.h"
#include "MainMenu.h"
#include "MySQL.h"
#include "UserInfo.h"
#include <vector>
#include <string>

class MultiGameScene : public Scene {

public:
    MultiGameScene(WindowManager& wm, SceneManager& manager);
    ~MultiGameScene();

    void drawInit() override;
    void handleEvents() override;
    void update() override;
    void render() override;

private:
    Print* print;
    WindowManager& windowManager;
    SceneManager& sceneManager;
    MultiGame* game;
};
