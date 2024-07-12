#include "Characters.h"

Characters::Characters() :
	data{ false }
{

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