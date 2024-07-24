#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <vector>
#include "Print.h"
#include "SceneManager.h"

class Characters : public Scene
{
public:

	Characters(WindowManager& wm, SceneManager& manager);
	enum { Width = 4, Height = 2 };
	void drawInit() override;
	void drawSelection();
	void handleArrowKey(SDL_Keycode key);
	void handleEvents() override;
	void update() override;
	void render() override;

private: 
	bool data[Width][Height];
	int sIndex = 0; 
	Print* print;
	WindowManager& windowManager;
	SceneManager& sceneManager;


};

