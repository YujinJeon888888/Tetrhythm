#include "TetrisScene.h"
#include "Game.h"
TetrisScene::TetrisScene(WindowManager& wm, SceneManager& manager)
	: windowManager(wm), sceneManager(manager), print(new Print(&wm)), game(new Game(windowManager))
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
	//�������� �ƴҶ��� ������Ʈ
	if (!game->tick())
	{
		game->tick();
	}
}

void TetrisScene::render()
{
	//Game.cpp���� ������ ������Ʈ�� �Ͼ�Ƿ� ���⼭ ȣ������ �ʽ��ϴ�
}
