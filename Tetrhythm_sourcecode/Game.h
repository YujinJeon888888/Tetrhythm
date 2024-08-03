#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Well.h"
#include "tetromino.h"
#include "WindowManager.h"
#include "Print.h"
class Game
{
public:
    Game(WindowManager& wm, Print* pr);
    ~Game();
    bool tick();
    bool isGameOver() const;
    SDL_Texture* getBlockTexture(Tetromino::Type type) const; // 블럭 텍스처 반환 함수 추가
private:
    Game(const Game&);
    Game& operator=(const Game&);
    Well well_;
    Tetromino tetromino_;
    uint32_t moveTime_;
    void check(const Tetromino&);
    int previousLine;
    int previousTetris;
    bool gameOver;
    SDL_Texture* blockTextures_[7]; // 7개의 블럭 텍스처를 저장하는 배열
    WindowManager& windowManager;
    Print* print;
};
