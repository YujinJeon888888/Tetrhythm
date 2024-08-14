#pragma once

#include "SceneManager.h"
#include "Game.h"
#include "TetrisScene.h"
#include "GameOverScene.h"
#include "MainMenu.h"
#include "MySQL.h"
#include "UserInfo.h"
#include "SoundManager.h" // SoundManager 헤더 포함
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
    SoundManager* soundManager; // SoundManager 객체 선언

    int heartPosX; // 하트의 현재 X 좌표
    int heartSpeed; // 하트의 이동 속도
    bool heartVisible; // 하트가 보이는지 여부
    bool musicPlayed;
    double timeSinceStart; // 게임 시작 후 경과 시간
    void deductHeart();

    std::vector<Heart> hearts;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point lastFrameTime;
};
