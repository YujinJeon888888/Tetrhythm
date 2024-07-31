#include "tetromino.h"
#include "Well.h" 

const int WELL_WIDTH = 10;
const int WELL_HEIGHT = 20;

const int BLOCK_SIZE = 25;

Tetromino::Tetromino(Type type) :
    type_(type),
    x_(WELL_WIDTH / 2 - 4 / 2),
    y_(0),
    angle_(0)
{}

void Tetromino::draw(SDL_Renderer* renderer, SDL_Texture* blockTexture)
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
