#pragma once
#include "Multi.h" //첫순서에 와야함
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


class LoadingScene : public Scene
{

public:
	LoadingScene(WindowManager& wm, SceneManager& manager); // 클래스 이름에 맞게 생성자 이름 변경 해줘야함.
	void drawInit() override;
	void handleEvents() override;
	void update() override;
	void render() override;
	void handleArrowKey(SDL_Keycode key);
private:
	Print* print;
	WindowManager& windowManager;
	SceneManager& sceneManager;
	
};
