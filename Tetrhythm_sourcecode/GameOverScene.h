#pragma once
#include "SceneManager.h"
#include "MainMenu.h"
class GameOverScene : public Scene {
public:
	GameOverScene(WindowManager& wm, SceneManager& manager); // 클래스 이름에 맞게 생성자 이름 변경 해줘야함.
	GameOverScene(WindowManager& wm, SceneManager& manager, int score, int line, int tetris); // 클래스 이름에 맞게 생성자 이름 변경 해줘야함.
	void drawInit() override;
	void handleEvents() override;
	void update() override;
	void render() override;
private:
	void handleArrowKey(SDL_Keycode key);
	Print* print;
	WindowManager& windowManager;
	SceneManager& sceneManager;
	int score;
	int line;
	int tetris;
};
