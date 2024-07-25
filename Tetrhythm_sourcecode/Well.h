#pragma once
#include "tetromino.h"
#include <SDL.h>

class Well
{
public:
    Well();
    void draw(SDL_Renderer*, SDL_Texture* blockTextures[]); // 텍스처 배열
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
    int line;
    int tetris;
};
