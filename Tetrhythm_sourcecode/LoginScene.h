#pragma once
#include "SceneManager.h"
#include "MainMenu.h"
#include "UserInfo.h"
#include "MySQL.h"
class LoginScene : public Scene {
public:
	LoginScene(WindowManager& wm, SceneManager& manager); 
	void drawInit() override;
	void handleEvents() override;
	void update() override;
	void render() override;
	void handleClosePopup(SDL_Keycode key);
	void whenSpace();
private:
	Print* print;
	WindowManager& windowManager;
	SceneManager& sceneManager;

};