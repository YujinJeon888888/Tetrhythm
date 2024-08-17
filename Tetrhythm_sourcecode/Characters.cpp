#include "Characters.h"

Characters::Characters(WindowManager& wm, SceneManager& manager) : windowManager(wm), sceneManager(manager), print(new Print(&wm)),
data{ false }
{
	 font = print->loadFont("DungGeunMo.ttf", 25);
	 color = { 255, 255, 255 }; // 흰색
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
		MySQL mysql;
		mysql.setCharacterName(UserInfo::getInstance().getUserID(), UserInfo::getInstance().getUserCharacter());
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

				print->deletePNG("openedLock 1.png");
				//오른쪽에 사진 띄우기 및 해금 조건 띄우기
				if (!data[x][y]) {
				
					print->printPNG("openedLock 1.png", 813, 152, 7);
					print->setText(10, unlockText());
				}
			
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
				switch (y)
				{
					MySQL mysql;
				case 0:
					switch (x)
					{
					case 0:
						UserInfo::getInstance().setUserCharacter("character" + std::to_string(x + 1) + ".png");
						break;
					case 1:
						UserInfo::getInstance().setUserCharacter("character" + std::to_string(x + 1) + ".png");						break;
					case 2:
						UserInfo::getInstance().setUserCharacter("character" + std::to_string(x + 1) + ".png");						break;
					case 3:
						UserInfo::getInstance().setUserCharacter("character" + std::to_string(x + 1) + ".png");						break;
					}
					break;
				case 1:
					switch (x)
					{
					case 0:
						UserInfo::getInstance().setUserCharacter("character" + std::to_string(x + 5) + ".png");						break;
					case 1:
						UserInfo::getInstance().setUserCharacter("character" + std::to_string(x + 5) + ".png");						break;
						break;
					case 2:
						UserInfo::getInstance().setUserCharacter("character" + std::to_string(x + 5) + ".png");						break;
						break;
					case 3:
						UserInfo::getInstance().setUserCharacter("character" + std::to_string(x + 5) + ".png");						break;
						break;
					}
					break;
				}

			}
		}

	//해금조건
	//윗줄
	if (sIndex <= 3) {
		//잠금상태면 해금 조건 표시
		if (!data[sIndex][0]) {
			print->printPNG("openedLock 1.png", 813, 142, 7);
			//텍스트
			switch (sIndex)
			{
			case 1:
				print->setText(10, "first multimode access");
				break;
			case 2:
				print->setText(10, "over 120 lines");
				break;
			case 3:
				print->setText(10, "over 50 tetrises");
				break;
			}
		}
		else {
			print->setText(10, " ");
			print->deleteLayer(7);
		}
	}
	else {//아랫줄
		//잠금상태면 해금 조건 표시
		if (!data[sIndex%4][1]) {
			print->printPNG("openedLock 1.png", 813, 142, 7);
			//텍스트
			switch (sIndex)
			{
			case 4:
				print->setText(10, "singlemode perfect clear");
				break;

			case 5:
				print->setText(10, "over 150,000 highscores");
				break;
			case 6:
				print->setText(10, "over 150 tetrises");
				break;
			case 7:
				print->setText(10, "over 300,000 highscores");
				break;
			}
		}
		else {
			print->setText(10, " ");
			print->deleteLayer(7);
		}
	}
}

// 최초에만 그려지는 고정된 요소들
void Characters::drawInit() {
	sIndex = UserInfo::getInstance().getUserCharacter().at(9) - '0' - 1;
	myCharIndex = sIndex;

    Print* pt = print;
    pt->printPNG("BackGround.png", 0, 0, 0);
	pt->printPNG("explanation.png", 351, 34, 2);

	unlock();

	for (auto x = 0; x < Width; ++x)
		for (auto y = 0; y < Height; y++) {


			std::ostringstream oss;
			oss << "character" << x + y * Width + 1 << ".png";
			std::string fileName = oss.str();

			pt->printPNG(fileName.c_str(), x * (128 + 33) + 122, y * (128 + 87) + 178, 3);

		//	pt->printPNG("CharacterSize.png", x * (128 + 33) + 122, y * (128 + 87) + 178, 2);

			if (!data[x][y]) {
				print->printPNG("lock.png", x * (128 + 33) + 233, y * (128 + 87) + 176, 7);

			}

		}


    //text
        //text
    //text 세팅
    //TTF_Font* font = print->loadFont("DungGeunMo.ttf", 25);
   // SDL_Color color = { 255, 255, 255 }; // 흰색
    print->printText("Line: ", 813, 526, 4, font, color);
    print->printText("Tetris: ",813, 556, 5, font, color);
    print->printText("HighScore: ",  813, 586, 6, font, color);
    //점수 text
    print->printText("      "+ std::to_string(UserInfo::getInstance().getLine()), 813, 526, 4, font, color);
    print->printText("        "+ std::to_string(UserInfo::getInstance().getTetris()), 813, 556, 5,font, color);
    print->printText("           "+ std::to_string(UserInfo::getInstance().getHighScore()),  813, 586, 6, font, color);
	//해금조건 text
	print->printText("Unlock Condition", 853, 152, 10, font, color);
}

void Characters::unlock() {

	//1. 자동 해금
	data[0][0] = true;

	//2. 첫 멀티모드 승리
	

	//3.총 120 lines
	MySQL mysql;
	if (UserInfo::getInstance().getLine() >= 120) {
		data[2][0] = true;
	}
	// 4. 총 50 Tetris
	if (UserInfo::getInstance().getTetris() >= 50) {
		data[3][0] = true;
	}
	//  5. 싱글모드 퍼펙트 클리어


    //  6. high score - 150000
	if (UserInfo::getInstance().getHighScore()>= 150000) {
		data[1][1] = true;
	}
    //  7. 총 150 Tetris
	if (UserInfo::getInstance().getTetris()>= 150) {
		data[2][1] = true;
	}
	// 8. high score - 300000
	if (UserInfo::getInstance().getHighScore()>= 300000) {
		data[3][1] = true;
	}
}

std::string Characters::unlockText() {


	switch (sIndex+1) {

	case 2: 
		return "첫 멀티 모드 승리";
	case 3:
		return "총 120 lines 달성";
	case 4:
		return "총 50 Tetris 달성";


	case 5:

		return "싱글 모드 percect clear";//"싱글 모드 퍼펙트 클리어";

	case 6:
		return "high score 150000 달성";
	case 7:
		return "총 150 Tetris 달성";
	case 8:
		return "high score 300000 달성";

	default: return "";
	}
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
