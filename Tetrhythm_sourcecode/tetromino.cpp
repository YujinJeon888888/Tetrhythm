#include "tetromino.h"
#include "Well.h"
#include <random>

const int WELL_WIDTH = 10;
const int WELL_HEIGHT = 20;
const int BLOCK_SIZE = 25;

std::mt19937 Tetromino::rng{ std::random_device{}() }; // 랜덤 엔진 초기화
std::uniform_int_distribution<int> Tetromino::dist(0, 6);

Tetromino::Tetromino() :
    type_(static_cast<Type>(dist(rng))), // 랜덤한 타입 선택
    x_(WELL_WIDTH / 2 - 2),
    y_(0),
    angle_(0)
{}

void Tetromino::draw(SDL_Renderer* renderer, SDL_Texture* blockTexture) const
{
    for (auto x = 0; x < 4; ++x)
        for (auto y = 0; y < 4; ++y)
            if (isBlock(x, y))
            {
                SDL_Rect rect{ 513 + (x + x_) * BLOCK_SIZE + 1, 116 + (y + y_) * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2 };
                SDL_RenderCopy(renderer, blockTexture, nullptr, &rect);
            }
}

void Tetromino::move(int dx, int dy)
{
    x_ += dx;
    y_ += dy;
}

void Tetromino::rotate()
{
    angle_ = (angle_ + 1) % 4;
}

void Tetromino::rotateCounterClockwise()
{
    angle_ = (angle_ + 3) % 4;
}

void Tetromino::drop(const Well& well)
{
    while (!well.isCollision(*this))
    {
        move(0, 1);
    }
    move(0, -1);
}

bool Tetromino::isBlock(int x, int y) const
{
    static const char* Shapes[][4] = {
      {
        " *  "
        " *  "
        " *  "
        " *  ",
        "    "
        "****"
        "    "
        "    ",
        " *  "
        " *  "
        " *  "
        " *  ",
        "    "
        "****"
        "    "
        "    ",
      },
      {
        "  * "
        "  * "
        " ** "
        "    ",
        "    "
        "*   "
        "*** "
        "    ",
        " ** "
        " *  "
        " *  "
        "    ",
        "    "
        "    "
        "*** "
        "  * ",
      },
      {
        " *  "
        " *  "
        " ** "
        "    ",
        "    "
        "*** "
        "*   "
        "    ",
        " ** "
        "  * "
        "  * "
        "    ",
        "  * "
        "*** "
        "    "
        "    ",
      },
      {
        "    "
        " ** "
        " ** "
        "    ",
        "    "
        " ** "
        " ** "
        "    ",
        "    "
        " ** "
        " ** "
        "    ",
        "    "
        " ** "
        " ** "
        "    ",
      },
      {
        "  * "
        " ** "
        " *  "
        "    ",
        "    "
        "**  "
        " ** "
        "    ",
        "  * "
        " ** "
        " *  "
        "    ",
        "    "
        "**  "
        " ** "
        "    ",
      },
      {
        " *  "
        " ** "
        " *  "
        "    ",
        "    "
        "*** "
        " *  "
        "    ",
        " *  "
        "**  "
        " *  "
        "    ",
        " *  "
        "*** "
        "    "
        "    ",
      },
      {
        " *  "
        " ** "
        "  * "
        "    ",
        "    "
        " ** "
        "**  "
        "    ",
        " *  "
        " ** "
        "  * "
        "    ",
        "    "
        " ** "
        "**  "
        "    ",
      },
    };
    return Shapes[type_][angle_][x + y * 4] == '*';
}

int Tetromino::x() const
{
    return x_;
}

int Tetromino::y() const
{
    return y_;
}

Tetromino::Type Tetromino::getType() const
{
    return type_;
}

Tetromino Tetromino::calculateShadow(const Well& well) const
{
    Tetromino shadow = *this; // 현재 블럭을 복제합니다.
    while (!well.isCollision(shadow))
    {
        shadow.move(0, 1); // 블럭을 한 칸씩 아래로 이동시킵니다.
    }
    shadow.move(0, -1); // 충돌한 마지막 위치에서 한 칸 위로 이동합니다.
    return shadow;
}
