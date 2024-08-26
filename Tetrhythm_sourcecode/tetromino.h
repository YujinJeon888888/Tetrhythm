#pragma once
#include <SDL.h>
#include <random> 
#include <vector>

class Well;

class Tetromino
{
public:
    enum Type { I = 0, J=1, L=2, O=3, S=4, T=5, Z=6, GRAY=7 };
    Tetromino(int xOffset = 513, int yOffset = 116);
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
    // 직렬화 함수
    void serialize(char* data) const {
        std::memcpy(data, &type_, sizeof(type_));
        std::memcpy(data + sizeof(type_), &x_, sizeof(x_));
        std::memcpy(data + sizeof(type_) + sizeof(x_), &y_, sizeof(y_));
        std::memcpy(data + sizeof(type_) + sizeof(x_) + sizeof(y_), &angle_, sizeof(angle_));
    }

    // 역직렬화 함수
    void deserialize(const char* data) {
        std::memcpy(&type_, data, sizeof(type_));
        std::memcpy(&x_, data + sizeof(type_), sizeof(x_));
        std::memcpy(&y_, data + sizeof(type_) + sizeof(x_), sizeof(y_));
        std::memcpy(&angle_, data + sizeof(type_) + sizeof(x_) + sizeof(y_), sizeof(angle_));
        xOffset = 61;
        yOffset = 100;
    }

private:
    Type type_;
    int x_;
    int y_;
    int angle_;
    int xOffset;  // 오프셋을 저장
    int yOffset;

    static std::mt19937 rng;
    static std::vector<Type> bag;   
    static int bagIndex;            

    static Type getNextType();     
};
