#pragma once
#include <SDL.h>
#include <random> // 랜덤 라이브러리 추가

class Well;

class Tetromino
{
public:
    enum Type { I = 0, J, L, O, S, T, Z };
    Tetromino();
    void draw(SDL_Renderer*, SDL_Texture* blockTexture) const;
    void move(int dx, int dy);
    void rotate();
    void rotateCounterClockwise();
    void drop(const Well& well);
    bool isBlock(int x, int y) const;
    int x() const;
    int y() const;
    Type getType() const; // 타입을 반환하는 함수 추가
    Tetromino calculateShadow(const Well& well) const; // 그림자 위치 계산 함수 추가
private:
    Type type_;
    int x_;
    int y_;
    int angle_;

    static std::mt19937 rng; // 랜덤 엔진
    static std::uniform_int_distribution<int> dist; // 0부터 6까지의 정수 분포
};
