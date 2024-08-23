#include "tetromino.h"
#include "Well.h"
#include <algorithm>  // std::shuffle 사용
#include <random>
#include <vector>

const int WELL_WIDTH = 10;
const int WELL_HEIGHT = 20;
const int BLOCK_SIZE = 25;

std::mt19937 Tetromino::rng{ std::random_device{}() };
std::vector<Tetromino::Type> Tetromino::bag;
int Tetromino::bagIndex = 0;

Tetromino::Tetromino(int xOffset, int yOffset) :
    type_(getNextType()),
    x_(WELL_WIDTH / 2 - 2),
    y_(0),
    angle_(0),
    xOffset(xOffset),  // 전달된 매개변수를 사용
    yOffset(yOffset)
{}
Tetromino::Type Tetromino::getNextType() {
    if (bagIndex >= 7) {
        bag = { I, J, L, O, S, T, Z };
        std::shuffle(bag.begin(), bag.end(), rng);
        bagIndex = 0; // 새로 섞었으므로 인덱스 초기화
    }
    return bag[bagIndex++];
}


void Tetromino::draw(SDL_Renderer* renderer, SDL_Texture* blockTexture) const
{
    for (auto x = 0; x < 4; ++x)
        for (auto y = 0; y < 4; ++y)
            if (isBlock(x, y))
            {
                SDL_Rect rect{ xOffset + (x + x_) * BLOCK_SIZE + 1, yOffset + (y + y_) * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2 };
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
    Tetromino shadow = *this;
    while (!well.isCollision(shadow))
    {
        shadow.move(0, 1);
    }
    shadow.move(0, -1);
    return shadow;
}
