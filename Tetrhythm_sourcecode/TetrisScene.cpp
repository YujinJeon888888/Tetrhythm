#include "TetrisScene.h"
#include "Game.h"
TetrisScene::TetrisScene(WindowManager& wm, SceneManager& manager)
	: windowManager(wm), sceneManager(manager), print(new Print(&wm)), game(new Game(windowManager,print))
{
	drawInit();
}

void TetrisScene::drawInit()
{
	//test
		//print->printPNG("CharacterSize.png", 0, 0, 1);
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
	//렌더링이 game안에 구현되어있길래, 그 안으로  renderForTetris함수를 넣어놨습니다.
	//고로 여기에 print->render()호출 x
}
