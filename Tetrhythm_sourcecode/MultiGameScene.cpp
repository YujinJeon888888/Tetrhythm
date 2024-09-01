#include "Multi.h"
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
    print->printPNG("heart1.png", 857, 50, 1);
    print->printPNG("heart2.png", 910, 50, 2);
    print->printPNG("heart3.png", 963, 50, 3);
    //플레이어쪽
    TTF_Font* font = print->loadFont("DungGeunMo.ttf", 25);
    TTF_Font* font2 = print->loadFont("DungGeunMo.ttf", 20);
    SDL_Color color = { 255, 255, 255 }; // 흰색
    print->printText("Line: ", 625, 388, 4, font, color);
    print->printText("Tetris: ", 625, 418, 5, font, color);
    print->printText("Score: ", 625, 448, 6, font, color);
    print->printText("      0", 625, 388, 7, font, color);
    print->printText("        0", 625, 418, 8, font, color);
    print->printText("       0", 625, 448, 9, font, color);
    print->printPNG(UserInfo::getInstance().getUserCharacter().c_str(), 654, 478, 13);
    //아이디: 가운데정렬로 출력
    int spaceCount = ((12 - UserInfo::getInstance().getUserID().size()) / 2)+1;
    std::string space = "";
    for (int i = 0; i < spaceCount; i++) {
        space += " ";
    }
    print->printText(space + UserInfo::getInstance().getUserID().c_str(), 654, 610, 12, font2, color); //플레이어ID
    //콤보 텍스트 
    TTF_Font* fontCombo = print->loadFont("DungGeunMo.ttf", 75, true);
    print->printText("Combo: ", 432, 62, 500, fontCombo, color);

    //상대방쪽
    print->printPNG("heart1o.png", 118, 50, 1);
    print->printPNG("heart2o.png", 171, 50, 2);
    print->printPNG("heart3o.png", 224, 50, 3);
    print->printText("Line: ", 329, 388, 4, font, color);
    print->printText("Tetris: ", 329, 418, 5, font, color);
    print->printText("Score: ", 329, 448, 6, font, color);
    print->printText("      0", 329, 388, 21, font, color);
    print->printText("        0", 329, 418, 22, font, color);
    print->printText("       0", 329, 448, 23, font, color);

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
        MySQL mysql;
        mysql.setLine(UserInfo::getInstance().getUserID(), (game->getLine()));
        mysql.setTetris(UserInfo::getInstance().getUserID(), (game->getTetris()));
        mysql.setHighScore(UserInfo::getInstance().getUserID(), (game->getScore()));
        UserInfo::getInstance().setScore(game->getScore());

        bool isLose = false;
        
        int type = Multi::getInstance()->receiveMessegeData();
        int flag = 0;
        while (type != 6 && flag < 5) {//점수
           type = Multi::getInstance()->receiveMessegeData();
           Multi::getInstance()->sendScore(game->getScore());
           flag++;
        }

        if (Multi::getInstance()->opponentScore > game->getScore())
            isLose = true;

        std::cout << " me: " << game->getScore() << "opp: " << Multi::getInstance()->opponentScore;

        if (game->getIsPerfectClear()) {
            //유저정보에 perfect clear저장.
            mysql.setPerfectClear(UserInfo::getInstance().getUserID(), (game->getIsPerfectClear()));
        }
        if (game->getIsClear()&& !isLose) {
            sceneManager.changeScene(std::make_unique<ClearScene>(windowManager, sceneManager, game->getLine(), game->getTetris(), game->getMaxCombo(), game->getIsClear()));
        }
        else {
            sceneManager.changeScene(std::make_unique<GameOverScene>(windowManager, sceneManager, game->getScore(), game->getLine(), game->getTetris(), game->getMaxCombo(), true));
        }

    }
}


void MultiGameScene::render()
{
    // `render` 함수에서 별도로 `print->moveImage`를 호출할 필요 없음
    // 하트 노드는 `update` 함수에서 이동 처리됨
}

