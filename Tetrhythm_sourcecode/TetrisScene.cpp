#include "TetrisScene.h"
#include "Game.h"
TetrisScene::TetrisScene(WindowManager& wm, SceneManager& manager)
	: windowManager(wm), sceneManager(manager), print(new Print(&wm)), game(new Game(windowManager,print))
{
	drawInit();
}

void TetrisScene::drawInit()
{
	print->printPNGForTetris("CharacterSize.png", 0, 0, 1);
}

void TetrisScene::handleEvents()
{
	print->handleEvents();
}


void TetrisScene::update()
{	
	//게임종료 아닐때만 업데이트
	if (!game->tick())
	{
		game->tick();
	}
}

void TetrisScene::render()
{

}
