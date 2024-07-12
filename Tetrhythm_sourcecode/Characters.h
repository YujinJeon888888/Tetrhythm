#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <vector>
#include "Print.h"

class Characters
{
public:
	Characters();
	enum { Width = 4, Height = 2 };
	void draw(Print* renderer);
	void draw(SDL_Renderer* renderer);
private: 
	bool data[Width][Height];

};

