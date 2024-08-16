#include "scoreTableScene.h"

scoreTableScene::scoreTableScene(WindowManager& wm, SceneManager& manager) : windowManager(wm), sceneManager(manager), print(new Print(&wm))
{
	drawInit();
}

void scoreTableScene::drawInit()
{
	MySQL mysql;
	std::string result = mysql.printTable();
	std::vector<std::string> tokens;
	tokens = splitString(result, '\n');
	TTF_Font* font = print->loadFont("Galmuri9.ttf", 18);
	SDL_Color color = { 255, 255, 255 }; // 흰색
	int y = 0;
	int x = 0;
	for (auto& token : tokens) {
		if (y >= 625) {
			y = 0;
			x += 425;
		}
		print->printText(token, x, y, 0, font, color);
		
		y += 25;
	}
}

void scoreTableScene::handleEvents()
{

}

void scoreTableScene::update()
{
	print->handleEvents([&](SDL_Event& event) {
		if (event.type == SDL_KEYDOWN) {
			handleArrowKey(event.key.keysym.sym);
		}
		});
}

void scoreTableScene::handleArrowKey(SDL_Keycode key) {

	switch (key) {
	case SDLK_SPACE:

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
