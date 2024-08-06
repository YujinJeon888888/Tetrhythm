#include "GameOverScene.h"

GameOverScene::GameOverScene(WindowManager& wm, SceneManager& manager) : windowManager(wm), sceneManager(manager), print(new Print(&wm))
{
	drawInit();
}

void GameOverScene::drawInit()
{
	print->printPNG("GameOver.png", 180, 50, 1);
	//text 세팅
	TTF_Font* font = print->loadFont("DungGeunMo.ttf", 30);
	SDL_Color color = { 255, 255, 255 }; // 흰색
	print->printText("Score: ", 442,430,print->getLayeredTextures().back().layer+1,font, color);
	print->printText("Line: ", 442,460,print->getLayeredTextures().back().layer+1,font, color);
	print->printText("Tetris: ", 442,490,print->getLayeredTextures().back().layer+1,font, color);

}

void GameOverScene::handleEvents()
{
	print->handleEvents();
}

void GameOverScene::update()
{
}

void GameOverScene::render()
{
	print->render();
}
