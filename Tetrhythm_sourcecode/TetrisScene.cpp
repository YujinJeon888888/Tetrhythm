#include "TetrisScene.h"
#include <iostream>
#include <chrono>

TetrisScene::TetrisScene(WindowManager& wm, SceneManager& manager)
    : windowManager(wm), sceneManager(manager), print(new Print(&wm)), game(new Game(windowManager, print)),
    heartPosX(70),  // 하트 노드의 시작 X 좌표
    heartSpeed(5),   // 하트 노드의 이동 속도
    heartVisible(false), // 하트 노드의 초기 상태 (숨겨짐)
    timeSinceStart(0.0), // 게임 시작 후 경과 시간
    lastFrameTime(std::chrono::steady_clock::now()), // 초기화 시점에서의 시간
    musicPlayed(false),
    soundManager(new SoundManager()) // SoundManager 객체 초기화
{
    drawInit();
}

TetrisScene::~TetrisScene() {
    soundManager->stopMusic(); // TetrisScene 객체가 파괴될 때 음악을 중지
    delete soundManager;
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

    print->printPNG("Background.png", 0, 0, 0); // 전체 배경
    print->printPNG("RhythmUIBackground.png", 70, 241, 10); // 리듬게임 UI 배경
}

void TetrisScene::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            sceneManager.quit();
        }

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_SPACE && heartVisible) {
                // heartPosX가 393 <= heartPosX <= 469 범위에 있지 않을 때
                if (!(heartPosX >= 393 && heartPosX <= 469)) {
                    // 하트를 하나 차감
                    deductHeart();
                }

                // 하트 노드를 즉시 삭제하고 상태를 업데이트합니다.
                print->deletePNG("heartNote.png");
                heartVisible = false; // 하트 노드가 사라졌음을 표시
                std::cout << "Heart Node Deleted by Spacebar at X: " << heartPosX << std::endl;
                // 시간 리셋해서 다음 하트 노드 생성 대기
                timeSinceStart = 3.0;
                lastFrameTime = std::chrono::steady_clock::now(); // 프레임 시간 리셋
            }
        }
    }

    print->handleEvents();
}

void TetrisScene::update() {
    // 현재 프레임 시간 계산
    auto currentFrameTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    // 경과 시간을 누적
    timeSinceStart += deltaTime.count();

    // 70 BPM 기준 한 박자의 길이(초)
    double beatInterval = 60.0 / 70.0;

    // 3초가 지나면 음악 재생
    if (timeSinceStart >= 3.0 && !musicPlayed) {
        soundManager->playMusic("Musics/Megalovania 8Bit Remix Audio.mp3", -1); // 음악 파일 경로와 반복 횟수 설정
        musicPlayed = true; // 음악 재생 완료 표시
        std::cout << "Music Started" << std::endl;
    }

    // 하트 노드가 보일 때 이동 시작
    if (heartVisible) {
        // 현재 위치 가져오기
        SDL_Rect rect = print->getImagePosition("heartNote.png");
        SDL_Point currentPosition = { rect.x, rect.y };

        // 한 마디(4박자)에 걸쳐 70에서 469까지 이동
        double totalDuration = 4 * beatInterval; // 4박자의 총 시간
        double moveDistance = (469 - 70) * (deltaTime.count() / totalDuration); // deltaTime을 이용한 부드러운 이동

        // 새로운 위치 계산
        int newPosX = currentPosition.x + moveDistance;

        // 하트 노드 이동
        print->moveImage("heartNote.png", newPosX, currentPosition.y);
        std::cout << "Heart Note Moved to X: " << newPosX << std::endl;

        // 배경 이미지의 오른쪽 끝에 도달했는지 체크
        if (newPosX >= 469) {
            heartVisible = false; // 하트 노드 사라짐
            print->deletePNG("heartNote.png");
            timeSinceStart = 3.0; // 시간 리셋해서 다음 하트 노드 생성 대기
            lastFrameTime = std::chrono::steady_clock::now(); // 프레임 시간 리셋
            std::cout << "Heart Note Deleted" << std::endl;
        }
    }
    else if (timeSinceStart >= 3.0 && !heartVisible) {
        // 하트 노드가 나타나기 시작
        heartVisible = true;
        heartPosX = 70; // 하트 노드의 초기 위치로 리셋
        print->printPNG("heartNote.png", heartPosX, 280, 11); // 초기 위치에 하트 노드 렌더링
        std::cout << "Heart Note Created at X: " << heartPosX << std::endl;
    }

    // 게임 상태 업데이트
    if (game->tick()) {
        game->tick();
    }
    else {
        // 게임 종료 시
        soundManager->stopMusic(); // 다른 창으로 이동하기 전에 음악을 중지합니다.
        MySQL mysql;
        mysql.setLine(UserInfo::getInstance().getUserID(), (game->getLine()));
        mysql.setTetris(UserInfo::getInstance().getUserID(), (game->getTetris()));

        sceneManager.changeScene(std::make_unique<GameOverScene>(windowManager, sceneManager, game->getScore(), game->getLine(), game->getTetris()));
    }
}


void TetrisScene::render()
{
    // `render` 함수에서 별도로 `print->moveImage`를 호출할 필요 없음
    // 하트 노드는 `update` 함수에서 이동 처리됨
}

// 하트 차감 함수 구현
void TetrisScene::deductHeart() {
    if (!hearts.empty()) {
        Heart lastHeart = hearts.back();
        print->deletePNG(lastHeart.path.c_str());
        hearts.pop_back();
        std::cout << "Heart deducted! Remaining hearts: " << hearts.size() << std::endl;
    }
    else {
        std::cout << "No hearts left to deduct" << std::endl;
    }
}
