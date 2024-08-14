#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <array>
#include "Well.h"
#include "tetromino.h"
#include "WindowManager.h"
#include "Print.h"
#include <vector>
#include <chrono>
#include <string>
#include "SoundManager.h" // SoundManager 헤더 포함


struct Heart {
    std::string path;
    int xPosition;
    int yPosition;
    static const std::string paths[3];
    static const int maxHeart = 3;
    static const int xPositions[3];
    static const int yPositions[3];
};



class Game
{
public:
    Game(WindowManager& wm, Print* pr);
    ~Game();
    bool tick();
    bool isGameOver() const;
    SDL_Texture* getBlockTexture(Tetromino::Type type) const; // 블럭 텍스처 반환 함수 추가
    
    int getLine() const{
        return previousLine;
    }
    int getTetris() const {
        return previousTetris;
    }
    int getScore() const {
        return score;
    }
private:
    Game(const Game&);
    Game& operator=(const Game&);
    Well well_;
    Tetromino tetromino_;
    std::array<Tetromino, 3> nextTetrominos_; // 대기열 블럭
    uint32_t moveTime_;
    void check(const Tetromino&);
    int previousLine;
    int previousTetris;
    int score;
    bool gameOver;
    SDL_Texture* blockTextures_[7]; // 7개의 블럭 텍스처를 저장하는 배열
    WindowManager& windowManager;
    Print* print;
    std::vector<Heart> hearts = {
       {Heart::paths[0], Heart::xPositions[0], Heart::yPositions[0]},
       {Heart::paths[1], Heart::xPositions[1], Heart::yPositions[1]},
       {Heart::paths[2], Heart::xPositions[2], Heart::yPositions[2]}
    };
    //하트로직 
    SoundManager* soundManager; // SoundManager 객체 선언
    int heartPosX; // 하트의 현재 X 좌표
    int heartSpeed; // 하트의 이동 속도
    bool heartVisible; // 하트가 보이는지 여부
    bool musicPlayed;
    double timeSinceStart; // 게임 시작 후 경과 시간
    void deductHeart();
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point lastFrameTime;

};
