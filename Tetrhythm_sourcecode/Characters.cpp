#include "Characters.h"

Characters::Characters(WindowManager& wm, SceneManager& manager) : windowManager(wm), sceneManager(manager), print(new Print(&wm)),
data{ false }, soundManager(new SoundManager())
{
	//TTF_Font* font = print->loadFont("DungGeunMo.ttf", 25);
	//SDL_Color color = { 255, 255, 255 }; // 흰색
	// data[0][3] = true;
	 //data[1][0] = true;
	myCharIndex = getXValueFromUserCharacter();
	sIndex = myCharIndex;
	drawInit();
}

int Characters::getXValueFromUserCharacter() {
	// 사용자 캐릭터의 경로를 가져옵니다.
	std::string userCharacter = UserInfo::getInstance().getUserCharacter();

	// 정규 표현식을 이용해 경로에서 숫자를 추출합니다.
	std::regex regex("Char_image/character(\\d+)\\.png");
	std::smatch match;

	if (std::regex_search(userCharacter, match, regex) && match.size() > 1) {
		// match[1]에 해당하는 부분이 숫자(x 값)입니다.
		std::cout << std::stoi(match.str(1)) << "charIndex";
		return std::stoi(match.str(1)) - 1; // x 값이므로 1을 빼줍니다.
	}

	// 해당 경로에서 숫자를 찾을 수 없는 경우, 적절한 오류 처리
	throw std::runtime_error("Invalid user character path");
}

void Characters::handleArrowKey(SDL_Keycode key) {

	switch (key) {
	case SDLK_UP:
		soundManager->loadSound("Musics/SelectionMove.mp3", "SelectionMove"); // 효과음 로드
		soundManager->playSound("SelectionMove", 0);


		if (sIndex > 3) sIndex -= 4;
		break;
	case SDLK_DOWN:
		soundManager->loadSound("Musics/SelectionMove.mp3", "SelectionMove"); // 효과음 로드
		soundManager->playSound("SelectionMove", 0);
		if (sIndex < 4) sIndex += 4;
		break;

	case SDLK_LEFT:
		soundManager->loadSound("Musics/SelectionMove.mp3", "SelectionMove"); // 효과음 로드
		soundManager->playSound("SelectionMove", 0);
		if (sIndex > 0) sIndex--;

		break;
	case SDLK_RIGHT:
		soundManager->loadSound("Musics/SelectionMove.mp3", "SelectionMove"); // 효과음 로드
		soundManager->playSound("SelectionMove", 0);
		if (sIndex < 7) sIndex++;

		break;

	case SDLK_SPACE: // 뒤로 가기 기능 추가
	case SDLK_RETURN:
		soundManager->loadSound("Musics/Selection.mp3", "Selection"); // 효과음 로드
		soundManager->playSound("Selection", 0);
		if (data[sIndex % Width][sIndex / Width])
			myCharIndex = sIndex;
		break;

	case SDLK_ESCAPE: // 뒤로 가기 기능 추가
		soundManager->loadSound("Musics/Selection.mp3", "Selection"); // 효과음 로드
		soundManager->playSound("Selection", 0);
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
				print->deletePNG("Resources/selection.png");
				print->printPNG("Resources/selection.png", posX, posY, 2);

				std::ostringstream oss;
				//오른쪽에 사진 띄우기 및 해금 조건 띄우기
				print->deleteLayer(100);
				if (!data[x][y]) {
					oss << "Char_image/character" << x + y * Width + 1 << "_Lu.png";
				}
				else {
					if (x == 0 && y == 0 && server1st) {
						oss << "Char_image/character9_L.png";
					}
					else {
						oss << "Char_image/character" << x + y * Width + 1 << "_L.png";
					}
				}


				std::string fileName = oss.str();
				print->printPNG(fileName.c_str(), 813, 207, 100);
			}

			if (x == myCharIndex % Width && myCharIndex / Width == y) {

				//std::cout << "\n?? :" << myCharIndex;
				int posX = x * (140 + 21) + 116; //5
				int posY = y * (140 + 76) + 171; //3

				// Delete and print the PNG at the calculated position
				//선택
				print->deletePNG("Resources/selectedRectang.png");
				print->printPNG("Resources/selectedRectang.png", posX, posY, 4);
				switch (y)
				{
					MySQL mysql;
				case 0:
					switch (x)
					{
					case 0:
						if (server1st) {
							UserInfo::getInstance().setUserCharacter("Char_image/character9.png");
						}
						else {
							UserInfo::getInstance().setUserCharacter("Char_image/character" + std::to_string(x + 1) + ".png");
						}
						break;
					case 1:
						UserInfo::getInstance().setUserCharacter("Char_image/character" + std::to_string(x + 1) + ".png");						break;
					case 2:
						UserInfo::getInstance().setUserCharacter("Char_image/character" + std::to_string(x + 1) + ".png");						break;
					case 3:
						UserInfo::getInstance().setUserCharacter("Char_image/character" + std::to_string(x + 1) + ".png");						break;
					}
					break;
				case 1:
					switch (x)
					{
					case 0:
						UserInfo::getInstance().setUserCharacter("Char_image/character" + std::to_string(x + 5) + ".png");						break;
					case 1:
						UserInfo::getInstance().setUserCharacter("Char_image/character" + std::to_string(x + 5) + ".png");						break;
						break;
					case 2:
						UserInfo::getInstance().setUserCharacter("Char_image/character" + std::to_string(x + 5) + ".png");						break;
						break;
					case 3:
						UserInfo::getInstance().setUserCharacter("Char_image/character" + std::to_string(x + 5) + ".png");						break;
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
			print->printPNG("Resources/openedLock 1.png", 813, 142, 7);
			//텍스트
			switch (sIndex)
			{
			case 1:
				print->setText(10, "first multimode win");
				break;
			case 2:
				print->setText(10, "over 120 lines");
				break;
			case 3:
				print->setText(10, "over 25 tetrises");
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
		if (!data[sIndex % 4][1]) {
			print->printPNG("Resources/openedLock 1.png", 813, 142, 7);
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
				print->setText(10, "over 50 tetrises");
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
	//내 캐릭터 호출

	//sIndex = UserInfo::getInstance().getUserCharacter().at(9) - '0' - 1;

	std::cout << sIndex << "charIndex";

	Print* pt = print;
	pt->printPNG("Resources/BackGround.png", 0, 0, 0);
	pt->printPNG("Resources/explanation.png", 351, 34, 2);
	pt->printPNG("Resources/openedLock 1.png", 813, 142, 7);

	unlock();

	for (auto x = 0; x < Width; ++x)
		for (auto y = 0; y < Height; y++) {


			std::ostringstream oss;
			if (!data[x][y]) {
				oss << "Char_image/character" << x + y * Width + 1 << "_su.png";
			}
			else {
				if (x == 0 && y == 0 && server1st) {
					oss << "Char_image/character9.png";
				}
				else {
					oss << "Char_image/character" << x + y * Width + 1 << ".png";
				}
			}
			std::string fileName = oss.str();

			pt->printPNG(fileName.c_str(), x * (128 + 33) + 122, y * (128 + 87) + 178, 3);

		}



	//text
		//text
	//text 세팅
	TTF_Font* font = print->loadFont("Resources/DungGeunMo.ttf", 25);
	SDL_Color color = { 255, 255, 255 }; // 흰색
	print->printText("Line: ", 777, 526, 4, font, color);
	print->printText("Tetris: ", 777, 556, 5, font, color);
	print->printText("HighScore: ", 777, 586, 6, font, color);
	//점수 text
	print->printText("      " + std::to_string(UserInfo::getInstance().getLine()), 777, 526, 8, font, color);
	print->printText("        " + std::to_string(UserInfo::getInstance().getTetris()), 777, 556, 8, font, color);
	print->printText("           " + std::to_string(UserInfo::getInstance().getHighScore()), 777, 586, 9, font, color);
	//해금조건 text
	print->printText("Unlock Condition", 853, 152, 10, font, color);

}

void Characters::unlock() {

	//1. 자동 해금
	data[0][0] = true;
	MySQL mysql;
	if (mysql.server1st(UserInfo::getInstance().getUserID())) {
		server1st = true;
	}


	//2. 첫 멀티모드 승리
	if (UserInfo::getInstance().getMultiModeWin()) {
		data[1][0] = true;
	}

	//3.총 120 lines
	if (UserInfo::getInstance().getLine() >= 120) {
		data[2][0] = true;
	}

	// 4. 총 25 Tetris
	if (UserInfo::getInstance().getTetris() >= 25) {
		data[3][0] = true;
	}

	//  5. 싱글모드 퍼펙트 클리어
	if (UserInfo::getInstance().getPerfectClear()) {
		data[0][1] = true;
	}

	//  6. high score - 150000
	if (UserInfo::getInstance().getHighScore() >= 150000) {
		data[1][1] = true;
	}
	//  7. 총 50 Tetris
	if (UserInfo::getInstance().getTetris() >= 50) {
		data[2][1] = true;
	}
	// 8. high score - 300000
	if (UserInfo::getInstance().getHighScore() >= 300000) {
		data[3][1] = true;
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
