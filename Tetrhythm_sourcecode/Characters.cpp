#include "Characters.h"

Characters::Characters(WindowManager& wm, SceneManager& manager) : windowManager(wm), sceneManager(manager), print(new Print(&wm)),
data{ false }
{
	drawInit();
}

void Characters::handleArrowKey(SDL_Keycode key) {

	switch (key) {
	case SDLK_UP:
		
	

		if (sIndex > 3) sIndex -= 4;
		break;
	case SDLK_DOWN:

		if (sIndex < 4) sIndex += 4;
		break;

	case SDLK_LEFT:

		if (sIndex > 0) sIndex--;

		break;
	case SDLK_RIGHT:

		if (sIndex < 7) sIndex++;

		break;

	case SDLK_SPACE: // 뒤로 가기 기능 추가
		if (data[sIndex % Width][sIndex / Width])
			myCharIndex = sIndex;
		break;

	case SDLK_ESCAPE: // 뒤로 가기 기능 추가
		sceneManager.goBack();

		break;
	}
}

// main에서 여러번 그려지는 함수
void Characters::drawSelection() {

	for (auto x = 0; x < Width; ++x)
		for (auto y = 0; y < Height; y++) {

			if (x == sIndex % Width && sIndex / Width == y) {
				int posX = x * (128 + 33) + 170;
				int posY = y * (128 + 87) + 178 - 29 - 27;

				// Delete and print the PNG at the calculated position
				print->deletePNG("selection.png");
				print->printPNG("selection.png", posX, posY, 2);

				//오른쪽에 사진 띄우기 및 해금 조건 띄우기


				std::ostringstream oss;
				oss << "character" << x + y * Width + 1 << "_s.png";
				std::string fileName = oss.str();
				print->deletePNG(fileName.c_str());
				print->printPNG(fileName.c_str(), 813, 207, 3);
			}

			if (x == myCharIndex % Width && myCharIndex / Width == y) {
				int posX = x * (140 + 21) + 116; //5
				int posY = y * (140 + 76) + 171; //3

				// Delete and print the PNG at the calculated position
				print->deletePNG("selectedRectang.png");
				print->printPNG("selectedRectang.png", posX, posY, 4);

			}
		}
}

// 최초에만 그려지는 고정된 요소들
void Characters::drawInit() {

    Print* pt = print;
    pt->printPNG("BackGround.png", 0, 0, 0);
	pt->printPNG("explanation.png", 351, 34, 2);
	pt->printPNG("openedLock 1.png", 853, 152, 7);

	unlock();

	for (auto x = 0; x < Width; ++x)
		for (auto y = 0; y < Height; y++) {


			std::ostringstream oss;
			oss << "character" << x + y * Width + 1 << ".png";
			std::string fileName = oss.str();

			pt->printPNG(fileName.c_str(), x * (128 + 33) + 122, y * (128 + 87) + 178, 3);

			pt->printPNG("CharacterSize.png", x * (128 + 33) + 122, y * (128 + 87) + 178, 2);

			if (!data[x][y]) {
				pt->printPNG("lock.png", x * (128 + 33) + 233, y * (128 + 87) + 176, 7);
			}


		}

    //text
        //text
    //text 세팅
    TTF_Font* font = print->loadFont("DungGeunMo.ttf", 25);
    SDL_Color color = { 255, 255, 255 }; // 흰색
    print->printText("Line: ", 777, 526, 4, font, color);
    print->printText("Tetris: ", 777, 556, 5, font, color);
    print->printText("Score: ", 777, 586, 6, font, color);
    //점수 text
    MySQL mysql;
    print->printText("      "+ std::to_string(mysql.getLine(UserInfo::getInstance().getUserID())), 777, 526, 7, font, color);
    print->printText("        "+ std::to_string(mysql.getTetris(UserInfo::getInstance().getUserID())), 777, 556, 8, font, color);
    print->printText("       0", 777, 586, 9, font, color);

}

void Characters::unlock() {

	//1. 자동 해금
	data[0][0] = true;

	//2. 첫 멀티모드 승리
	data[1][0] = true;

	//3.총 120 lines

	// 4. 총 50 Tetris


	//  5. 싱글모드 퍼펙트 클리어

    //  6. high score - 150000

	  //  7. 총 150 Tetris

	   // 8. high score - 300000
}

void Characters::handleEvents() {
	print->handleEvents([&](SDL_Event& event) {
		if (event.type == SDL_KEYDOWN) {
			handleArrowKey(event.key.keysym.sym);
		}
		});
}

void Characters::update() {
	// 업데이트 로직 추가

}

void Characters::render() {
	drawSelection();
	print->updateAnimations();
	print->render();
}
