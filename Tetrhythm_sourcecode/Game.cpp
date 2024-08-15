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


bool spaceLock = false;
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
    moveTime_(SDL_GetTicks() + 1000), // 초기화 시 블록이 1초 후에 내려오도록 설정
    previousLine(0),
    previousTetris(0),
    score(0),
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
    static Uint32 lastMoveTime = 0;
    static Uint32 lastRotationTime = 0;
    static Uint32 lastDropTime = 0;

    Uint32 currentTime = SDL_GetTicks();
    Uint32 moveDelay = 100;   // 이동 시 딜레이 (밀리초 단위)
    Uint32 rotateDelay = 200; // 회전 시 딜레이 (밀리초 단위)
    Uint32 dropDelay = 150;   // 드랍 시 딜레이 (밀리초 단위)

    if (hearts.size() == 0)
    {
        gameOver = true;
        std::cout << "Game Over!" << std::endl;
    }

    if (gameOver) {
        soundManager->stopMusic(); // 다른 창으로 이동하기 전에 음악을 중지합니다.
        return false;
    }

    // 이벤트 처리
    SDL_Event e;
    if (SDL_WaitEventTimeout(&e, 10))
    {
        switch (e.type)
        {
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
            case SDLK_DOWN:
                if (currentTime > lastMoveTime + moveDelay)
                {
                    Tetromino t = tetromino_;
                    t.move(0, 1);
                    if (!well_.isCollision(t))
                        tetromino_ = t;
                    moveTime_ = SDL_GetTicks() + 500; // 아래로 이동 시 자동 내려오는 시간을 조정
                    lastMoveTime = currentTime; // 마지막 이동 시간 기록
                }
                break;

            case SDLK_RIGHT:
            case SDLK_LEFT:
                if (currentTime > lastMoveTime + moveDelay)
                {
                    Tetromino t = tetromino_;
                    t.move((e.key.keysym.sym == SDLK_RIGHT) ? 1 : -1, 0);
                    if (!well_.isCollision(t))
                        tetromino_ = t;
                    lastMoveTime = currentTime; // 마지막 이동 시간 기록
                }
                break;

            case SDLK_a:
                if (currentTime > lastRotationTime + rotateDelay)
                {
                    Tetromino t = tetromino_;
                    t.rotate();
                    if (!well_.isCollision(t))
                        tetromino_ = t;
                    lastRotationTime = currentTime; // 마지막 회전 시간 기록
                }
                break;

            case SDLK_d:
                if (currentTime > lastRotationTime + rotateDelay)
                {
                    Tetromino t = tetromino_;
                    t.rotateCounterClockwise();
                    if (!well_.isCollision(t))
                        tetromino_ = t;
                    lastRotationTime = currentTime; // 마지막 회전 시간 기록
                }
                break;

            case SDLK_SPACE:
                if (currentTime > lastDropTime + dropDelay)
                {
                    Tetromino t = tetromino_;
                    t.drop(well_);
                    check(t);
                    lastDropTime = currentTime; // 마지막 드랍 시간 기록

                    // 추가: 하트 노트 위치 판정
                    if (heartVisible)
                    {
                        if (393 < heartPosX && heartPosX < 469)
                        {
                            score += (heartPosX == 432) ? 1500 : 500;
                            print->setText(9, "       " + std::to_string(score));
                            std::cout << "safe!" << std::endl;
                            heartVisible = false;
                            print->deletePNG("heartNote.png");
                        }
                        else if (heartPosX <= 393)
                        {
                            deductHeart();
                        }
                    }
                }
                break;

            }
            break;

        case SDL_QUIT:
            return false;
        }
    }

    // 프레임 타이밍 관리
    auto currentFrameTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;
    timeSinceStart += deltaTime.count();

    // 자동으로 블럭이 내려오도록 설정
    if (SDL_GetTicks() > moveTime_)
    {
        Tetromino t = tetromino_;
        t.move(0, 1);
        if (!well_.isCollision(t))
        {
            tetromino_ = t;
            moveTime_ = SDL_GetTicks() + 1000; // 다음 내려오는 시간 설정
        }
        else
        {
 
            check(t);
        }
    }
    int currentLine = well_.getLine();
    int currentTetris = well_.getTetris();

    if (currentLine > previousLine)
    {
        std::cout << "Line: " << currentLine << std::endl;
        previousLine = currentLine;
        print->setText(7, "      " + std::to_string(previousLine));
    }

    //테트리스
    if (currentTetris > previousTetris)
    {
        //하트 맥시멈(3)보다 작을때만, 목숨 추가.
        if (hearts.size() < Heart::maxHeart && hearts.size() != 0) {
            Heart heart{ Heart::paths[hearts.size()], Heart::xPositions[hearts.size()], Heart::yPositions[hearts.size()] };
            hearts.push_back(heart);
            print->printPNG(heart.path.c_str(), heart.xPosition, heart.yPosition);
        }

        std::cout << "Tetris: " << currentTetris << std::endl;
        previousTetris = currentTetris;
        print->setText(8, "        " + std::to_string(previousTetris));

    }


    int currentLine = well_.getLine();
    int currentTetris = well_.getTetris();

    if (currentLine > previousLine)
    {
        std::cout << "Line: " << currentLine << std::endl;
        previousLine = currentLine;
        print->setText(7, "      " + std::to_string(previousLine));
    }

    //테트리스
    if (currentTetris > previousTetris)
    {
        //하트 맥시멈(3)보다 작을때만, 목숨 추가.
        if (hearts.size() < Heart::maxHeart && hearts.size() != 0) {
            Heart heart{ Heart::paths[hearts.size()], Heart::xPositions[hearts.size()], Heart::yPositions[hearts.size()] };
            hearts.push_back(heart);
            print->printPNG(heart.path.c_str(), heart.xPosition, heart.yPosition);
        }

        std::cout << "Tetris: " << currentTetris << std::endl;
        previousTetris = currentTetris;
        print->setText(8, "        " + std::to_string(previousTetris));

    }

    // 화면 업데이트
    SDL_SetRenderDrawColor(windowManager.getRenderer(), 0, 0, 0, 0xff);
    SDL_RenderClear(windowManager.getRenderer());
    print->renderForTetris();

    // 그림자 위치 계산 및 그리기
    Tetromino shadow = tetromino_.calculateShadow(well_);
    well_.drawShadow(windowManager.getRenderer(), blockTextures_[tetromino_.getType()], shadow);
    well_.draw(windowManager.getRenderer(), blockTextures_, nextTetrominos_);
    tetromino_.draw(windowManager.getRenderer(), blockTextures_[tetromino_.getType()]);

    // 하트노트 움직임
    double beatInterval = 60.0 / 70.0;
    if (timeSinceStart >= 3.0 && !musicPlayed)
    {
        soundManager->playMusic("Musics/Megalovania 8Bit Remix Audio.mp3", -1);
        musicPlayed = true;
    }

    if (heartVisible)
    {
        SDL_Rect rect = print->getImagePosition("heartNote.png");
        SDL_Point currentPosition = { rect.x, rect.y };
        double totalDuration = 4 * beatInterval;
        double moveDistance = (469 - 70) * (deltaTime.count() / totalDuration);
        int newPosX = currentPosition.x + moveDistance;

        heartPosX = newPosX;
        print->moveImage("heartNote.png", heartPosX, currentPosition.y);

        if (heartPosX >= 469)
        {
            deductHeart();
            heartVisible = false;
            print->deletePNG("heartNote.png");
            timeSinceStart = 3.0;
            lastFrameTime = std::chrono::steady_clock::now();
        }
    }
    else if (timeSinceStart >= 3.0 && !heartVisible)
    {
        heartVisible = true;
        heartPosX = 70;
        print->printPNG("heartNote.png", heartPosX, 280, 11);
    }

    SDL_RenderPresent(windowManager.getRenderer());

    return true;
}

void Game::check(const Tetromino& t)
{
    if (t.y() >= 0 && well_.isCollision(t))
    {

        well_.unite(tetromino_);  // 블록을 well에 추가

        // 블록이 추가된 후 새로운 블록을 생성합니다.
        tetromino_ = nextTetrominos_[0];
        nextTetrominos_[0] = nextTetrominos_[1];
        nextTetrominos_[1] = nextTetrominos_[2];
        nextTetrominos_[2] = Tetromino{};


        // 새로운 블록이 Well에 충돌하면 게임 오버 처리
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

void Game::deductHeart()
{
    std::cout << "when heartPosX : " << heartPosX << "deduct heart" << std::endl;
    if (!hearts.empty())
    {
        Heart lastHeart = hearts.back();
        print->deletePNG(lastHeart.path.c_str());
        hearts.pop_back();
        std::cout << "Heart deducted! Remaining hearts: " << hearts.size() << std::endl;
    }
    else
    {
        std::cout << "No hearts left to deduct" << std::endl;
    }
}
