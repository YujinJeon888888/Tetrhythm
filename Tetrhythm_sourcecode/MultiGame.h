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
#include "MainMenu.h"
#include <string>
#include "SoundManager.h" // SoundManager 헤더 포함
#include "SceneManager.h"


struct MultiHeart {
    std::string path;
    int xPosition;
    int yPosition;
    static const std::string paths[3];
    static const int maxHeart = 3;
    static const int xPositions[3];
    static const int yPositions[3];
    static const int xPositions_opponent[3];

};



class MultiGame
{
public:
    MultiGame(WindowManager& wm, Print* pr, SceneManager& sm);
    ~MultiGame();
    bool tick();
    bool isGameOver() const;
    SDL_Texture* getBlockTexture(Tetromino::Type type) const; // 블럭 텍스처 반환 함수 추가

    int getLine() const {
        return previousLine;
    }
    int getTetris() const {
        return previousTetris;
    }
    int getScore() const {
        return score;
    }
    bool getIsClear() const {
        return isClear;
    }
    bool getIsPerfectClear() const {
        return isPerfectClear;
    }
    int getMaxCombo() const {
        return maxCombo;
    }
private:
    MultiGame(const MultiGame&);
    MultiGame& operator=(const MultiGame&);
    Well well_; //플레이어 보드
    Well opponentWell_; //상대방 보드
    Tetromino tetromino_;
    std::array<Tetromino, 3> nextTetrominos_; // 대기열 블럭
    uint32_t moveTime_;
    void check(const Tetromino&);
    int previousLine;
    int previousTetris;
    int score=0;
    bool gameOver;
    SDL_Texture* blockTextures_[7]; // 7개의 블럭 텍스처를 저장하는 배열
    SDL_Texture* grayBlockTexture_;
    WindowManager& windowManager;
    Print* print;
    std::vector<MultiHeart> hearts = {
       {MultiHeart::paths[0], MultiHeart::xPositions[0], MultiHeart::yPositions[0]},
       {MultiHeart::paths[1], MultiHeart::xPositions[1], MultiHeart::yPositions[1]},
       {MultiHeart::paths[2], MultiHeart::xPositions[2], MultiHeart::yPositions[2]}
    };

    std::vector<MultiHeart> oppnentHearts = {
     {MultiHeart::paths[0], MultiHeart::xPositions_opponent[0], MultiHeart::yPositions[0]},
     {MultiHeart::paths[1], MultiHeart::xPositions_opponent[1], MultiHeart::yPositions[1]},
     {MultiHeart::paths[2], MultiHeart::xPositions_opponent[2], MultiHeart::yPositions[2]}
    };
    //하트로직 
    SoundManager* soundManager; // SoundManager 객체 선언
    int heartPosX; // 하트의 현재 X 좌표
    int heartSpeed; // 하트의 이동 속도
    bool heartVisible; // 하트가 보이는지 여부
    bool musicPlayed;
    double timeSinceStart; // 게임 시작 후 경과 시간
    double heartSpawnInterval; // 140 BPM 4/4박자마다 생성 간격 (초 단위)
    double nextHeartSpawnTime; // 다음 하트 노드 생성 타이밍
    void deductHeart();
    void deductHeart_opponent();
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point lastFrameTime;
    SceneManager& sceneManager;
    bool isClear = false;
    bool isPerfectClear = true;

    int fullComboCount = 0;
    //
    int seriesTetrisCount = 0;
    const int comboScore = 100000;
    int comboCount = 0;
    std::vector<int> comboVector;
    int forDebugCount = 0;
    //비트!
    double beatInterval = 60.0 / 140.0;
    //하트 깎이는 거 한 번 체크
    bool heartDeduct = false;
    bool countdown3Displayed = false;
    bool countdown2Displayed = false;
    bool countdown1Displayed = false;
    bool startDisplayed = false;
    bool startDeleted = false;
    double perfectImageStartTime; // Perfect.png 표시 시점 기록
    bool perfectImageVisible;
    int spacePressed=0;
    int maxCombo;
};
