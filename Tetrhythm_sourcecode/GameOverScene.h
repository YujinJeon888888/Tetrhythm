#pragma once
#include "SceneManager.h"
class GameOverScene : public Scene {
public:
	GameOverScene(WindowManager& wm, SceneManager& manager); // 클래스 이름에 맞게 생성자 이름 변경 해줘야함.
	void drawInit() override;
	void handleEvents() override;
	void update() override;
	void render() override;
private:
	Print* print;
	WindowManager& windowManager;
	SceneManager& sceneManager;
};
