#include "Characters.h"

Characters::Characters() :
	data{ false }
{

}

void Characters::handleArrowKey(SDL_Keycode key) {
    switch (key) {
    case SDLK_UP:
        std::cout << "Up arrow key pressed!" << std::endl;
        break;
    case SDLK_DOWN:
        std::cout << "Down arrow key pressed!" << std::endl;
        break;
    case SDLK_LEFT:
        std::cout << "Left arrow key pressed!" << std::endl;
        break;
    case SDLK_RIGHT:
        std::cout << "Right arrow key pressed!" << std::endl;
        break;
    }
}

void Characters::draw(Print* pt) {


	pt->printPNG("explanation.png", 351,34, 2);
	data[1][0] = true;

	for (auto x = 0; x < Width; ++x)
		for (auto y = 0; y < Height; y++) {

			if (data[x][y]) {


				pt->printPNG("selection.png", x * (128 + 33) + 170, y * (128 + 87) + 178 -29-27, 2);

			}
			
				pt->printPNG("CharacterSize.png", x * (128+33) + 122, y * (128 + 87) + 178, 2);
			
		}


}