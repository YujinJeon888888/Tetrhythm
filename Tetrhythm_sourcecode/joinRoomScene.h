

#pragma once
#include "MutiMenuScene.h"
#include "SceneManager.h"

class joinRoomScene :public Scene
{
	public:
		joinRoomScene(WindowManager& wm, SceneManager& manager);
		void drawInit() override;
		void handleEvents() override;
		void update() override;
		void render() override;
		void whenSpace();
		void handleArrowKey(SDL_Keycode key);
	private:
		Print* print;
		WindowManager& windowManager;
		SceneManager& sceneManager;

};

