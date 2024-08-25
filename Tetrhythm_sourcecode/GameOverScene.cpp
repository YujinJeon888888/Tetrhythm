#include "GameOverScene.h"



GameOverScene::GameOverScene(WindowManager& wm, SceneManager& manager, int score, int line, int tetris, int combo)
	: windowManager(wm), sceneManager(manager), print(new Print(&wm)), score(score), line(line), tetris(tetris), soundManager(new SoundManager()), combo(combo)
{
	drawInit();
	soundManager->loadSound("Musics/YouLose.mp3", "YouLose"); // 효과음 로드
	soundManager->playSound("YouLose", 0);
}


void GameOverScene::drawInit()
{
	print->printPNG("BackGround.png", 0, 0, 0);
	print->printPNG("GameOver.png", 180, 50, 1);
	//text 세팅
	TTF_Font* font = print->loadFont("DungGeunMo.ttf", 30);
	SDL_Color color = { 255, 255, 255 }; // 흰색
	print->printText("Score: " + std::to_string(score), 442,430,print->getLayeredTextures().back().layer+1,font, color);
	print->printText("Line: " + std::to_string(line), 442,460,print->getLayeredTextures().back().layer+1,font, color);
	print->printText("Tetris: " + std::to_string(tetris), 442,490,print->getLayeredTextures().back().layer+1,font, color);
	print->printText("Combo: " + std::to_string(combo), 442,520,print->getLayeredTextures().back().layer+1,font, color);
}

void GameOverScene::handleEvents()
{
	print->handleEvents([&](SDL_Event& event) {
		if (event.type == SDL_KEYDOWN) {
			handleArrowKey(event.key.keysym.sym);
		}
		});
}

void GameOverScene::handleArrowKey(SDL_Keycode key) {
    switch (key) {
    case SDLK_ESCAPE: // 뒤로 가기 기능 추가
		sceneManager.changeScene(std::make_unique<MainMenu>(windowManager, sceneManager));
		break;
    }
}


void GameOverScene::update()
{
}

void GameOverScene::render()
{
	print->render();
}
