#include "TetrisScene.h"
#include <iostream>
#include <chrono>

TetrisScene::TetrisScene(WindowManager& wm, SceneManager& manager)
    : windowManager(wm), sceneManager(manager), print(new Print(&wm)), game(new Game(windowManager, print)),
    heartPosX(70),  // 하트 노드의 시작 X 좌표
    heartSpeed(5),   // 하트 노드의 이동 속도
    heartVisible(false), // 하트 노드의 초기 상태 (숨겨짐)
    timeSinceStart(0.0), // 게임 시작 후 경과 시간
    musicPlayed(false), 
    soundManager(new SoundManager()) // SoundManager 객체 초기화
{
    drawInit();

    // 시작 시간을 기록합니다.
    startTime = std::chrono::steady_clock::now();
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
                heartVisible = false;
                print->deletePNG("heartNote.png");
                std::cout << "Heart Node Deleted by Spacebar" << std::endl;
                // 점수 추가 로직을 여기에 넣을 수 있음!
            }
        }
    }

    print->handleEvents();
}

void TetrisScene::update()
{
    // 실제 경과 시간 계산
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = currentTime - startTime;
    timeSinceStart = elapsed.count(); // 경과 시간을 초 단위로 가져옴

    // 70BPM 기준 한 박자의 길이(초)
    double beatInterval = 60.0 / 70.0;

    // 3초가 지나면 음악 재생
    if (timeSinceStart >= 3.0 && !musicPlayed) {
        soundManager->playMusic("Musics/Megalovania 8Bit Remix Audio.mp3", -1); // 음악 파일 경로와 반복 횟수 설정
        musicPlayed = true; // 음악 재생 완료 표시
        std::cout << "Music Started" << std::endl;
    }

    // 3초가 지나면 하트 노드가 나타나기 시작
    if (timeSinceStart >= 3.0 && !heartVisible) {
        heartVisible = true;                                            
        heartPosX = 70; // 하트 노드의 초기 위치로 리셋
        print->printPNG("heartNote.png", heartPosX, 280, 11); // 초기 위치에 하트 노드 렌더링
        std::cout << "Heart Node Created at X: " << heartPosX << std::endl;
    }

    // 3초가 지나고 하트 노드가 보일 때 이동 시작
    if (heartVisible) {
        double distancePerFrame = 424.0 / (60.0 * 4 * beatInterval); // 70BPM, 4/4 박자
        double frameDuration = 1.0 / 60.0; // 한 프레임의 시간 (60 FPS 기준)
        heartPosX += distancePerFrame * frameDuration;

        // 하트 노드 이동
        print->moveImage("heartNote.png", heartPosX, 280);
        std::cout << "Heart Node Moved to X: " << heartPosX << std::endl;

        // 배경 이미지의 오른쪽 끝에 도달했는지 체크
        if (heartPosX >= 469) {
            heartVisible = false; // 하트 노드 사라짐
            print->deletePNG("heartNote.png");
            startTime = std::chrono::steady_clock::now(); // 시간 리셋해서 다음 하트 노드 생성 대기
            std::cout << "Heart Node Deleted" << std::endl;
        }
    }

    // 게임 상태 업데이트
    if (game->tick()) {
        game->tick();
    }
    else {
        // 게임 종료 시
        MySQL mysql;
        mysql.setLine(UserInfo::getInstance().getUserID(), (game->getLine()));
        mysql.setTetris(UserInfo::getInstance().getUserID(), (game->getTetris()));

        sceneManager.changeScene(std::make_unique<GameOverScene>(windowManager, sceneManager, game->getScore(), game->getLine(), game->getTetris()));
    }
}

void TetrisScene::render()
{
    // 하트 노드가 보일 때 하트 노드 그리기
    //if (heartVisible) {
    //    print->moveImage("heartNote.png", heartPosX, 280); // 하트 노드 이미지 파일을 사용하세요
    //}
}
