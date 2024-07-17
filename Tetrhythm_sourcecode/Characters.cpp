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
        data[sIndex % Width][sIndex / Width] = false;
        if (sIndex > 0) sIndex--;
       // std::cout << "Left arrow key pressed!" << std::endl;
        break;
    case SDLK_RIGHT:
        data[sIndex % Width][sIndex / Width] = false;
        if (sIndex < 7) sIndex++;
     //   std::cout << "Right arrow key pressed!" << std::endl;
        break;
    }
}

// main에서 여러번 그려지는 함수
void Characters::drawSelection(Print* pt) {


	data[sIndex % Width][sIndex  / Width] = true;
   // std::cout << sIndex;
	for (auto x = 0; x < Width; ++x)
		for (auto y = 0; y < Height; y++) {
	
			if (data[x][y]) {
                int posX = x * (128 + 33) + 170;
                int posY = y * (128 + 87) + 178 - 29 - 27;

                // Delete and print the PNG at the calculated position
                pt->deletePNG("selection.png");
                pt->printPNG("selection.png", posX, posY, 2);
			}
		
			
		}


}
// 최초에만 그려지는 고정된 요소들
void Characters::drawInit(Print* pt) {

	pt->printPNG("explanation.png", 351, 34, 2);
	data[sIndex % Width][sIndex / Width] = true;

	for (auto x = 0; x < Width; ++x)
		for (auto y = 0; y < Height; y++) {

			if (data[x][y]) {

				pt->printPNG("selection.png", x * (128 + 33) + 170, y * (128 + 87) + 178 - 29 - 27, 2);

			}

			pt->printPNG("CharacterSize.png", x * (128 + 33) + 122, y * (128 + 87) + 178, 3);

		}


}