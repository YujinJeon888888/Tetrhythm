#include "TetrisScene.h"
#include <iostream>
#include <chrono>

TetrisScene::TetrisScene(WindowManager& wm, SceneManager& manager)
    : windowManager(wm), sceneManager(manager), print(new Print(&wm)), game(new Game(windowManager, print)),
    heartPosX(100),  // 하트 노드의 시작 X 좌표
    heartSpeed(5),   // 하트 노드의 이동 속도
    heartVisible(false), // 하트 노드의 초기 상태 (숨겨짐)
    timeSinceStart(0.0) // 게임 시작 후 경과 시간

{
    drawInit();
}

void TetrisScene::drawInit()
{
	//test
		//print->printPNG("CharacterSize.png", 0, 0, 1);
	//목숨 초기 프린트.
	print->printPNG("heart1.png", 570, 43, 1);
	print->printPNG("heart2.png", 623, 43, 2);
	print->printPNG("heart3.png", 676, 43, 3);
	//text
	//text 세팅
	TTF_Font* font = print->loadFont("DungGeunMo.ttf", 25);
	SDL_Color color = { 255, 255, 255 }; // 흰색
	print->printText("Line: " , 777, 526, 4, font, color);
	print->printText("Tetris: " , 777, 556, 5, font, color);
	print->printText("Score: " , 777, 586, 6, font, color);
	//점수 text
	print->printText("      0", 777, 526, 7, font, color);
	print->printText("        0", 777, 556,8, font, color);
	print->printText("       0", 777, 586, 9, font, color);
	//캐릭터
	print->printPNG(UserInfo::getInstance().getUserCharacter().c_str(), 317, 429, 10);
    // 기존 배경 및 UI 설정
    print->printPNG("Background.png", 0, 0, 0); // 전체 배경
    print->printPNG("RhythmUIBackground.png", 70, 241, 10); // 리듬게임 UI 배경
}

void TetrisScene::handleEvents()
{
    print->handleEvents();
}

void TetrisScene::update()
{
    // 경과 시간 업데이트
    timeSinceStart += 0.016; // assuming 60 FPS (1/60 ≈ 0.016)

    // 2초가 지나면 하트 노드가 나타나기 시작
    if (timeSinceStart >= 2.0) {
        if (heartVisible) {
            heartPosX += heartSpeed;

            // 배경 이미지의 오른쪽 끝에 도달했는지 체크
            if (heartPosX >= 524) { // 100(시작 위치) + 424(배경의 너비)
                heartVisible = false; // 하트 노드 사라짐
            }
        }
        else {
            // 하트 노드가 사라졌다면 다시 초기화
            heartVisible = true;
            heartPosX = 100; // 하트 노드의 초기 위치로 리셋
        }
    }

    // 140 BPM에 맞추어 하트 노드 애니메이션 주기 설정
    double beatInterval = 60.0 / 140.0; // 140 BPM의 비트 간격 (초)
    if (static_cast<int>(timeSinceStart / beatInterval) % 2 == 0) {
        if (heartVisible) {
            heartPosX += heartSpeed;

            // 배경 이미지의 오른쪽 끝에 도달했는지 체크
            if (heartPosX >= 524) { // 100(시작 위치) + 424(배경의 너비)
                heartVisible = false; // 하트 노드 사라짐
            }
        }
    }

    // 게임 상태 업데이트
    if (game->tick()) {
        game->tick();
    }
    else {
        // 게임 종료 시
        // 1. 데이터 저장 (line, tetris)
        MySQL mysql;
        mysql.setLine(UserInfo::getInstance().getUserID(), (game->getLine()));
        mysql.setTetris(UserInfo::getInstance().getUserID(), (game->getTetris()));

        // 2. 게임 오버 씬 이동
        sceneManager.changeScene(std::make_unique<GameOverScene>(windowManager, sceneManager, game->getScore(), game->getLine(), game->getTetris()));
    }
}

void TetrisScene::render()
{
    
    // 하트 노드가 보일 때 하트 노드 그리기
    if (heartVisible)
    {
        print->printPNG("heartNode.png", heartPosX, 370, 1); // 하트 노드 이미지 파일을 사용하세요
    }
}
