#pragma once
#include "SceneManager.h"
#include "MainMenu.h"
#include "SoundManager.h"

class ClearScene : public Scene{
public:
	ClearScene(WindowManager& wm, SceneManager& manager, int line, int tetris);
	void drawInit() override;
	void handleEvents() override;
	void update() override;
	void render() override;
private:
	Print* print;
	WindowManager& windowManager;
	SceneManager& sceneManager;
	void handleArrowKey(SDL_Keycode key);
	int line;
	int tetris;
	SoundManager* soundManager;

};