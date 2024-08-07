#pragma once
#include "SceneManager.h"
#include "Game.h"
#include "TetrisScene.h"
#include "GameOverScene.h"
#include "MainMenu.h"
#include "MySQL.h"
#include "UserInfo.h"
class TetrisScene : public Scene {
public:
	TetrisScene(WindowManager& wm, SceneManager& manager); // 클래스 이름에 맞게 생성자 이름 변경 해줘야함.
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