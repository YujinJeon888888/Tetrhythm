#pragma once
#include "tetromino.h"
#include <SDL.h>
#include <array>

class Well
{
public:
    Well(int xOffset = 513, int yOffset = 116, int queueXOffset = 806, int queueYOffset = 130);
    void draw(SDL_Renderer* renderer, SDL_Texture* blockTextures[], const std::array<Tetromino, 3>& nextTetrominos); // 텍스처 배열 및 다음 블럭들
    void drawShadow(SDL_Renderer* renderer, SDL_Texture* blockTexture, const Tetromino& shadow) const; // 그림자 그리기 함수 추가
    enum { Width = 10, Height = 20 };
    bool isCollision(const Tetromino& t) const;
    void unite(const Tetromino&);
    int getLine() const;
    int getTetris() const;
private:
    bool data[Width][Height];
    Tetromino::Type dataTypes[Width][Height]; // 각 블럭의 타입을 저장하는 배열
    int xOffset;
    int yOffset;
    int queueXOffset;  // 블럭 대기열의 X 오프셋
    int queueYOffset;  // 블럭 대기열의 Y 오프셋
    int line;
    int tetris;
};
