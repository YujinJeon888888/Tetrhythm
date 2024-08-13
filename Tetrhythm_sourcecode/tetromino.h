#pragma once
#include <SDL.h>
#include <random> 
#include <vector>

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
    Type getType() const;
    Tetromino calculateShadow(const Well& well) const;

private:
    Type type_;
    int x_;
    int y_;
    int angle_;

    static std::mt19937 rng;
    static std::vector<Type> bag;   
    static int bagIndex;            

    static Type getNextType();     
};
