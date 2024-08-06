#include "TetrisScene.h"
TetrisScene::TetrisScene(WindowManager& wm, SceneManager& manager)
	: windowManager(wm), sceneManager(manager), print(new Print(&wm)), game(new Game(windowManager,print))
{
	drawInit();
}

void TetrisScene::drawInit()
{
	//test
		//print->printPNG("CharacterSize.png", 0, 0, 1);
	//목숨 초기 프린트.
	print->printPNG("heart1.png", 570, 43, 1);
	print->printPNG("heart2.png", 623, 43, print->getLayeredTextures().back().layer + 1);
	print->printPNG("heart3.png", 676, 43, print->getLayeredTextures().back().layer + 1);

}

void TetrisScene::handleEvents()
{
	print->handleEvents();
}


void TetrisScene::update()
{	
	//게임종료 아닐때만 업데이트
	if (game->tick())
	{
		game->tick();
	}
	else {
		//게임종료일 때
		//1. 데이터저장(line, tetris)
		MySQL mysql;
		mysql.setLine(UserInfo::getInstance().getUserID(), (game->getLine()));
		mysql.setTetris(UserInfo::getInstance().getUserID(), (game->getTetris()));
		//2. 게임오버 씬 이동
		sceneManager.pushScene(std::make_unique<MainMenu>(windowManager, sceneManager));
		sceneManager.changeScene(std::make_unique<GameOverScene>(windowManager, sceneManager,game->getScore(),game->getLine(),game->getTetris()));
	}
	//게임 클리어일 때
	//1. 데이터 저장
	//2. 게임클리어 씬 이동
}
	


void TetrisScene::render()
{
	//렌더링이 game안에 구현되어있길래, 그 안으로  renderForTetris함수를 넣어놨습니다.
	//고로 여기에 print->render()호출 x
}
