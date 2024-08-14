#include "Game.h"
#include <stdexcept>
#include <iostream>
#include "tetromino.h"
#include "Well.h"
#include "Print.h"
#include "WindowManager.h"
#include <vector>
#include <thread>
#include <chrono>

const std::string Heart::paths[3] = {
    "heart1.png",
    "heart2.png",
    "heart3.png"
};
const int Heart::xPositions[3] = {
    570,
    623,
    676
};
const int Heart::yPositions[3] = {
    43,
    43,
    43
};

Game::Game(WindowManager& wm, Print* pr)
    :
    tetromino_{},
    nextTetrominos_{ Tetromino{}, Tetromino{}, Tetromino{} }, // 다음 블럭 3개 초기화
    moveTime_(SDL_GetTicks()),
    previousLine(0),
    previousTetris(0),
    gameOver(false),
    windowManager(wm),
    print(pr), 
    heartPosX(70),  // 하트 노드의 시작 X 좌표
    heartSpeed(5),   // 하트 노드의 이동 속도
    heartVisible(false), // 하트 노드의 초기 상태 (숨겨짐)
    timeSinceStart(0.0), // 게임 시작 후 경과 시간
    lastFrameTime(std::chrono::steady_clock::now()), // 초기화 시점에서의 시간
    musicPlayed(false),
    soundManager(new SoundManager()) // SoundManager 객체 초기화
{
    // 7개의 텍스처 로드
    const char* textureFiles[7] = {
        "Skyblue_I.png",
        "Blue_J.png",
        "Orange_L.png",
        "Yellow_O.png",
        "Green_S.png",
        "Purple_T.png",
        "Red_Z.png"
    };

    for (int i = 0; i < 7; ++i)
    {
        SDL_Surface* surface = IMG_Load(textureFiles[i]);
        if (!surface)
        {
            throw std::runtime_error("Failed to load image: " + std::string(IMG_GetError()));
        }
        blockTextures_[i] = SDL_CreateTextureFromSurface(windowManager.getRenderer(), surface);
        SDL_FreeSurface(surface);
        if (!blockTextures_[i])
        {
            throw std::runtime_error("Failed to create texture: " + std::string(SDL_GetError()));
        }
    }
}

Game::~Game()
{
    soundManager->stopMusic(); // TetrisScene 객체가 파괴될 때 음악을 중지
    delete soundManager;

    for (int i = 0; i < 7; ++i)
    {
        if (blockTextures_[i])
        {
            SDL_DestroyTexture(blockTextures_[i]);
        }
    }
}

bool Game::tick()
{
    if (gameOver) {
        soundManager->stopMusic(); // 다른 창으로 이동하기 전에 음악을 중지합니다.
        return false;
    }
    SDL_Event e;
    if (SDL_WaitEventTimeout(&e, 250))
    {
        switch (e.type)
        {
        case SDL_KEYDOWN:
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_DOWN:
            {
                Tetromino t = tetromino_;
                t.move(0, 1);
                if (!well_.isCollision(t))
                    tetromino_ = t;
            }
            break;
            case SDLK_RIGHT:
            {
                Tetromino t = tetromino_;
                t.move(1, 0);
                if (!well_.isCollision(t))
                    tetromino_ = t;
            }
            break;
            case SDLK_LEFT:
            {
                Tetromino t = tetromino_;
                t.move(-1, 0);
                if (!well_.isCollision(t))
                    tetromino_ = t;
            }
            break;
            case SDLK_a:
            {
                Tetromino t = tetromino_;
                t.rotate();
                if (!well_.isCollision(t))
                    tetromino_ = t;
            }
            break;
            case SDLK_d:
            {
                Tetromino t = tetromino_;
                t.rotateCounterClockwise();
                if (!well_.isCollision(t))
                    tetromino_ = t;
            }
            break;
            case SDLK_SPACE:
            {
                Tetromino t = tetromino_;
                t.drop(well_);
                check(t);
                if (heartVisible) {
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
            break;
            }
        }
        break;
        case SDL_QUIT:
            return false;
        }
    }
    SDL_SetRenderDrawColor(windowManager.getRenderer(), 0, 0, 0, 0xff);
   

    //화면업데이트
    SDL_RenderClear(windowManager.getRenderer());
    
    print->renderForTetris();
    // 그림자 위치 계산
    Tetromino shadow = tetromino_.calculateShadow(well_);

    // 그림자 그리기
    well_.drawShadow(windowManager.getRenderer(), blockTextures_[tetromino_.getType()], shadow);

    // 기존 그리기 코드...
    well_.draw(windowManager.getRenderer(), blockTextures_, nextTetrominos_);
    tetromino_.draw(windowManager.getRenderer(), blockTextures_[tetromino_.getType()]);


    if (SDL_GetTicks() > moveTime_)
    {
        moveTime_ += 1000;
        Tetromino t = tetromino_;
        t.move(0, 1);
        check(t);
    }

    
    //화면업데이트
    SDL_RenderPresent(windowManager.getRenderer());

    int currentLine = well_.getLine();
    int currentTetris = well_.getTetris();

    if (currentLine > previousLine)
    {
        std::cout << "Line: " << currentLine << std::endl;
        previousLine = currentLine;
        print->setText(7,"      "+std::to_string(previousLine));
    }

    //테트리스
    if (currentTetris > previousTetris)
    {
        //하트 맥시멈(3)보다 작을때만, 목숨 추가.
        if (hearts.size() < Heart::maxHeart && hearts.size()!=0) {
            Heart heart{ Heart::paths[hearts.size()], Heart::xPositions[hearts.size()], Heart::yPositions[hearts.size()] };
            hearts.push_back(heart);
            print->printPNG(heart.path.c_str(), heart.xPosition, heart.yPosition);
        }

        std::cout << "Tetris: " << currentTetris << std::endl;
        previousTetris = currentTetris;
        print->setText(8, "        " + std::to_string(previousTetris));

    }
    //하트노트 로직
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
    return true;
}

void Game::check(const Tetromino& t)
{
    if (t.y() >= 0 && well_.isCollision(t)) // 블럭이 Well 내부에 위치할 때만 충돌 검사
    //목숨이 0인지 체크
    if (hearts.size()== 0) {
        gameOver = true;
        std::cout << "Game Over!" << std::endl;
    }
    if (well_.isCollision(t))
    {
        well_.unite(tetromino_);
        tetromino_ = nextTetrominos_[0]; // 현재 블럭을 대기열의 첫 번째 블럭으로 교체
        nextTetrominos_[0] = nextTetrominos_[1]; // 대기열 이동
        nextTetrominos_[1] = nextTetrominos_[2]; // 대기열 이동
        nextTetrominos_[2] = Tetromino{}; // 새로운 랜덤 블럭 생성

        // 대기열 블럭이 한 칸씩 당겨지며 0.2초 지연 후 위로 이동
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        if (well_.isCollision(tetromino_))
        {
            gameOver = true;
            std::cout << "Game Over!" << std::endl;
        }
    }
    else
    {
        tetromino_ = t;
    }
}

bool Game::isGameOver() const
{
    return gameOver;
}

SDL_Texture* Game::getBlockTexture(Tetromino::Type type) const
{
    return blockTextures_[type];
}


// 하트 차감 함수 구현
void Game::deductHeart() {
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