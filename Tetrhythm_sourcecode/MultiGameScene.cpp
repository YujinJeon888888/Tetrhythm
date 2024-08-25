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
void receiveAndPrintOpponentData(Print* print) {
    TTF_Font* font2 = print->loadFont("DungGeunMo.ttf", 20);
    SDL_Color color = { 255, 255, 255 }; // 흰색

    try {
        std::pair<std::string, std::string> opponentData = Multi::getInstance()->receiveIDAndCharacter();
        std::string opponentID = opponentData.first;
        std::string opponentCharacter = opponentData.second;

        if (opponentID != "ERROR" && opponentCharacter != "ERROR") {
            std::cout << "Opponent ID: " << opponentID << std::endl;
            std::cout << "Opponent Character Image: " << opponentCharacter << std::endl;
        }
        else {
            std::cout << "Failed to receive opponent data" << std::endl;
        }

       // std::cout << id << cimg;
        print->printPNG(opponentID.c_str(), 329, 478, 11);     // 상대방 캐릭터 사진
        print->printText(opponentCharacter.c_str(), 327, 610, 12, font2, color); // 상대방 ID 출력

    }
    catch (const std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << std::endl;
    }
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
    print->printText("Line: ", 650, 388, 4, font, color);
    print->printText("Tetris: ", 650, 418, 5, font, color);
    print->printText("Score: ", 650, 448, 6, font, color);
    print->printText("      0", 650, 388, 7, font, color);
    print->printText("        0", 650, 418, 8, font, color);
    print->printText("       0", 650, 448, 9, font, color);
    print->printPNG(UserInfo::getInstance().getUserCharacter().c_str(), 654, 478, 13);
    print->printText(UserInfo::getInstance().getUserID().c_str(), 654, 610, 12, font2, color); //플레이어ID

    Multi::getInstance()->sendID(UserInfo::getInstance().getUserCharacter(), UserInfo::getInstance().getUserID());

    // 스레드를 생성하고 실행
    std::thread opponentThread(receiveAndPrintOpponentData, print);

    // 스레드가 완료될 때까지 기다림 (병렬 작업이 아니면 join() 필요)
    opponentThread.detach();
    //상대방쪽
    print->printPNG("heart1.png", 118, 50, 1);
    print->printPNG("heart2.png", 171, 50, 2);
    print->printPNG("heart3.png", 224, 50, 3);
    print->printText("Line: ", 329, 388, 4, font, color);
    print->printText("Tetris: ", 329, 418, 5, font, color);
    print->printText("Score: ", 329, 448, 6, font, color);
 
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
            sceneManager.changeScene(std::make_unique<ClearScene>(windowManager, sceneManager, game->getLine(), game->getTetris(),game->getMaxCombo(),game->getIsClear()));
        }
        else {
       
           sceneManager.changeScene(std::make_unique<GameOverScene>(windowManager, sceneManager, game->getScore(), game->getLine(), game->getTetris(),game->getMaxCombo()));
        }
    }
}


void MultiGameScene::render()
{
    // `render` 함수에서 별도로 `print->moveImage`를 호출할 필요 없음
    // 하트 노드는 `update` 함수에서 이동 처리됨
}

