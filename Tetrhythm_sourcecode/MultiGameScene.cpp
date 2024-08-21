#include "MultiGameScene.h"


MultiGameScene::MultiGameScene(WindowManager& wm, SceneManager& manager)
    : windowManager(wm), sceneManager(manager), print(new Print(&wm)), game(new MultiGame(windowManager, print, sceneManager))
{
    drawInit();
}

MultiGameScene::~MultiGameScene() {
    delete print;
    delete game;
}

void MultiGameScene::drawInit()
{
    // 기존 UI 및 배경 이미지 설정
    print->printPNG("heart1.png", 118, 50, 1);
    print->printPNG("heart2.png", 171, 50, 2);
    print->printPNG("heart3.png", 224, 50, 3);
    //플레이어쪽
    TTF_Font* font = print->loadFont("DungGeunMo.ttf", 25);
    TTF_Font* font2 = print->loadFont("DungGeunMo.ttf", 20);
    SDL_Color color = { 255, 255, 255 }; // 흰색
    print->printText("Line: ", 329, 388, 4, font, color);
    print->printText("Tetris: ", 329, 418, 5, font, color);
    print->printText("Score: ", 329, 448, 6, font, color);
    print->printText("      0", 329, 388, 7, font, color);
    print->printText("        0", 329, 418, 8, font, color);
    print->printText("       0", 329, 448, 9, font, color);
    print->printPNG(UserInfo::getInstance().getUserCharacter().c_str(), 329, 478, 11);
    print->printText(UserInfo::getInstance().getUserID().c_str(), 327, 610, 12, font2, color); //플레이어ID



    //상대방쪽
    print->printText("Line: ", 650, 388, 4, font, color);
    print->printText("Tetris: ", 650, 418, 5, font, color);
    print->printText("Score: ", 650, 448, 6, font, color);
    //상대방 캐릭터 사진 (바꿔야됨)
    print->printPNG(UserInfo::getInstance().getUserCharacter().c_str(), 654, 478, 13);
    print->printText(UserInfo::getInstance().getUserID().c_str(), 654, 610, 12, font2, color); //상대방ID(바꿔야됨)


    print->printPNG("Background.png", 0, 0, 0); // 전체 배경
    print->printPNG("MultiRhythmUI.png", 475, 186, 10); // 리듬게임 UI 배경
}

void MultiGameScene::handleEvents()
{

    print->handleEvents();
}

void MultiGameScene::update() {


    // 게임 상태 업데이트
    if (game->tick()) {
        game->tick();
    }
    else {
        // 게임 종료 시
       
        if (game->getIsClear()) {
            sceneManager.changeScene(std::make_unique<ClearScene>(windowManager, sceneManager, game->getLine(), game->getTetris()));
        }
        else {
            sceneManager.changeScene(std::make_unique<GameOverScene>(windowManager, sceneManager, game->getScore(), game->getLine(), game->getTetris()));
        }
    }
}


void MultiGameScene::render()
{
    // `render` 함수에서 별도로 `print->moveImage`를 호출할 필요 없음
    // 하트 노드는 `update` 함수에서 이동 처리됨
}

