#pragma once
#include <SDL.h>

class Well;

class Tetromino
{
public:
    enum Type { I = 0, J, L, O, S, T, Z };
    Tetromino(Type);
    void draw(SDL_Renderer*, SDL_Texture* blockTexture);
    void move(int dx, int dy);
    void rotate();
    void rotateCounterClockwise();
    void drop(const Well& well);
    bool isBlock(int x, int y) const;
    int x() const;
    int y() const;
    Type getType() const; // 타입을 반환하는 함수 추가
private:
    Type type_;
    int x_;
    int y_;
    int angle_;
};
