#pragma once
#include "tetromino.h"
#include <SDL.h>
#include <array>

class Well
{
public:
    Well(int xOffset = 513, int yOffset = 116, int queueXOffset = 806, int queueYOffset = 130);
    // 세 개의 인수를 받는 draw 메서드
    void draw(SDL_Renderer* renderer, SDL_Texture* blockTextures[], SDL_Texture* grayBlockTexture, const std::array<Tetromino, 3>& nextTetrominos);
    // 기존 두 개의 인수를 받는 draw 메서드
    void draw(SDL_Renderer* renderer, SDL_Texture* blockTextures[], const std::array<Tetromino, 3>& nextTetrominos);

    void drawShadow(SDL_Renderer* renderer, SDL_Texture* blockTexture, const Tetromino& shadow) const;
    enum { Width = 10, Height = 20 };
    bool isCollision(const Tetromino& t) const;
    void unite(const Tetromino&);
    int getLine() const;
    int getTetris() const;
    void addGrayLines(int numLines, bool Gap);
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
