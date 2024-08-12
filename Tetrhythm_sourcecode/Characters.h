#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <vector>
#include "Print.h"
#include "SceneManager.h"
#include "UserInfo.h"
#include <sstream>


class Characters : public Scene
{
public:

	Characters(WindowManager& wm, SceneManager& manager);
	void drawInit() override;
	void handleEvents() override;
	void update() override;
	void render() override;

	enum { Width = 4, Height = 2 };
	void drawSelection();
	void handleArrowKey(SDL_Keycode key);
	

private: 
	bool data[Width][Height];
	int sIndex = 0; 
	int myCharIndex = 0;
	void unlock();
	std::string unlockText();
	Print* print;
	WindowManager& windowManager;
	SceneManager& sceneManager;
	TTF_Font* font;               
	SDL_Color color;

};

