#include "scoreTableScene.h"
MySQL mysql;
std::string result;
std::vector<std::string> tokens;
TTF_Font* font;
SDL_Color color = { 255, 255, 255 }; // 흰색
std::vector<SDL_Color> colors = {
	{255, 0, 0},      // 빨강
	{255, 165, 0},    // 주황
	{255, 255, 0},    // 노랑
	{0, 255, 0},      // 초록
	{0, 0, 255},      // 파랑
	{75, 0, 130},     // 남색 (인디고)
	{238, 130, 238}   // 보라 (바이올렛)
};
scoreTableScene::scoreTableScene(WindowManager& wm, SceneManager& manager) : windowManager(wm), sceneManager(manager), print(new Print(&wm))
{
	drawInit();
}

void scoreTableScene::drawInit()
{
	//프린트 초기화
	print->clearScreen();
	print->deleteLayer(0);
	print->deleteLayer(1);
	result = mysql.printTable();
	tokens = splitString(result, '\n');
	font = print->loadFont("DungGeunMo.ttf", 18);

	int y = 0;
	int x = 0;
	for (auto& token : tokens) {
		if (y >= 625) {
			y = 0;
			x += 200;
		}
		if (y == 0 && x == 0) {
			// 애니메이션 초기화
			print->textAnimation(token, x, y, font, colors, 8, 1);
		}
		else {
			print->printText(token, x, y, 0, font, color);
		}

		y += 25;
	}
	print->render();
}

void scoreTableScene::handleEvents()
{
	print->handleEvents();
}

void scoreTableScene::update()
{
	print->updateTextAnimation();
	print->handleEvents([&](SDL_Event& event) {
		if (event.type == SDL_KEYDOWN) {
			handleArrowKey(event.key.keysym.sym);
		}
		});
}

void scoreTableScene::handleArrowKey(SDL_Keycode key) {

	switch (key) {
	case SDLK_SPACE:
		drawInit();
		break;
	}
}

void scoreTableScene::render()
{
	print->render();
}

//std::vector<std::string> splitString(const std::string& str, char delimiter) {
//	std::vector<std::string> tokens;
//	std::stringstream ss(str);
//	std::string token;
//
//	while (std::getline(ss, token, delimiter)) {
//		tokens.push_back(token);
//	}
//
//	return tokens;
//}
