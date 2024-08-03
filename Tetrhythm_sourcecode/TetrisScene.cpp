#include "TetrisScene.h"
#include "Game.h"
TetrisScene::TetrisScene(WindowManager& wm, SceneManager& manager)
	: windowManager(wm), sceneManager(manager), print(new Print(&wm)), game(new Game(windowManager,print))
{
	
}

void TetrisScene::drawInit()
{
	
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
	//Game.cpp에서 렌더링 업데이트가 일어나므로 여기서 호출하지 않습니다
}
