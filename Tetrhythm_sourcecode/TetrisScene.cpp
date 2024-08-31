
#include "TetrisScene.h"


TetrisScene::TetrisScene(WindowManager& wm, SceneManager& manager)
    : windowManager(wm), sceneManager(manager), print(new Print(&wm)), game(new Game(windowManager, print, sceneManager))
{
    drawInit();
}

TetrisScene::~TetrisScene() {
    delete print;
    delete game;
}

void TetrisScene::drawInit()
{
    // 기존 UI 및 배경 이미지 설정
    print->printPNG("heart1.png", 570, 43, 1);
    print->printPNG("heart2.png", 623, 43, 2);
    print->printPNG("heart3.png", 676, 43, 3);

    TTF_Font* font = print->loadFont("DungGeunMo.ttf", 25);
    SDL_Color color = { 255, 255, 255 }; // 흰색
    print->printText("Line: ", 777, 526, 4, font, color);
    print->printText("Tetris: ", 777, 556, 5, font, color);
    print->printText("Score: ", 777, 586, 6, font, color);
    print->printText("      0", 777, 526, 7, font, color);
    print->printText("        0", 777, 556, 8, font, color);
    print->printText("       0", 777, 586, 9, font, color);
    print->printPNG(UserInfo::getInstance().getUserCharacter().c_str(), 317, 429, 10);
    int spaceCount= ((12-UserInfo::getInstance().getUserID().size())/2)-1;
    std::string space = "";
    for (int i = 0; i < spaceCount; i++) {
        space += " ";
    }
    print->printText(space+UserInfo::getInstance().getUserID(), 317, 570, 10, font, color);


    print->printPNG("Background.png", 0, 0, 0); // 전체 배경
    print->printPNG("RhythmUIBackground.png", 70, 241, 10); // 리듬게임 UI 배경
}

void TetrisScene::handleEvents()
{

    print->handleEvents();
}

void TetrisScene::update() {


    // 게임 상태 업데이트
    if (game->tick()) {
        game->tick();
    }
    else {
        // 게임 종료 시
        MySQL mysql;
        mysql.setLine(UserInfo::getInstance().getUserID(), (game->getLine()));
        mysql.setTetris(UserInfo::getInstance().getUserID(), (game->getTetris()));
        mysql.setHighScore(UserInfo::getInstance().getUserID(), (game->getScore()));
        UserInfo::getInstance().setScore(game->getScore());

        if (game->getIsPerfectClear()) {
            //유저정보에 perfect clear저장.
            mysql.setPerfectClear(UserInfo::getInstance().getUserID(), (game->getIsPerfectClear()));
        }
        if (game->getIsClear()) {
            sceneManager.changeScene(std::make_unique<ClearScene>(windowManager, sceneManager,game->getLine(),game->getTetris(),game->getMaxCombo()));
        }
        else {
            sceneManager.changeScene(std::make_unique<GameOverScene>(windowManager, sceneManager, game->getScore(), game->getLine(), game->getTetris(),game->getMaxCombo(),false));
        }
    }
}


void TetrisScene::render()
{
    // `render` 함수에서 별도로 `print->moveImage`를 호출할 필요 없음
    // 하트 노드는 `update` 함수에서 이동 처리됨
}

