#include "Well.h"
#include "Print.h"

// 각 블럭의 크기
const int BLOCK_SIZE = 25;

Well::Well() :
    data{ {false} },
    xOffset(513),
    yOffset(116),
    line(0),
    tetris(0)
{
    for (int x = 0; x < Width; ++x)
        for (int y = 0; y < Height; ++y)
            dataTypes[x][y] = Tetromino::Type::I; // 초기값
}

void Well::draw(SDL_Renderer* renderer, SDL_Texture* blockTextures[])
{
    for (auto x = 0; x < Width; ++x)
        for (auto y = 0; y < Height; ++y)
        {
            if (data[x][y])
            {
                SDL_Rect rect{ xOffset + x * BLOCK_SIZE + 1, yOffset + y * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2 };
                SDL_RenderCopy(renderer, blockTextures[dataTypes[x][y]], nullptr, &rect);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderDrawPoint(renderer, xOffset + x * BLOCK_SIZE + BLOCK_SIZE / 2, yOffset + y * BLOCK_SIZE + BLOCK_SIZE / 2);
            }
        }
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

    int wellWidth = Width * BLOCK_SIZE + 5;
    int wellHeight = Height * BLOCK_SIZE + 5;

    SDL_Rect bottomFrame = { xOffset, yOffset + wellHeight - 5, wellWidth, 5 };
    SDL_RenderFillRect(renderer, &bottomFrame);

    SDL_Rect leftFrame = { xOffset - 5 , yOffset, 5, wellHeight };
    SDL_RenderFillRect(renderer, &leftFrame);

    SDL_Rect rightFrame = { xOffset + wellWidth - 5, yOffset, 5, wellHeight };
    SDL_RenderFillRect(renderer, &rightFrame);
}

bool Well::isCollision(const Tetromino& t) const
{
    for (int x = 0; x < 4; ++x)
        for (int y = 0; y < 4; ++y)
            if (t.isBlock(x, y))
            {
                auto wx = t.x() + x;
                auto wy = t.y() + y;
                if (wx < 0 || wx >= Width || wy < 0 || wy >= Height)
                    return true;
                if (data[wx][wy])
                    return true;
            }
    return false;
}

void Well::unite(const Tetromino& t)
{
    for (int x = 0; x < 4; ++x)
        for (int y = 0; y < 4; ++y)
            if (t.isBlock(x, y))
            {
                data[t.x() + x][t.y() + y] = true;
                dataTypes[t.x() + x][t.y() + y] = t.getType(); // 블럭 타입 저장
            }

    std::vector<int> fullLines;
    for (int y = 0; y < Height; ++y)
    {
        bool solid = true;
        for (int x = 0; x < Width; ++x)
        {
            if (!data[x][y])
            {
                solid = false;
                break;
            }
        }
        if (solid)
        {
            fullLines.push_back(y);
        }
    }

    line += fullLines.size();

    if (fullLines.size() == 4)
    {
        tetris += 1;
    }

    for (int line : fullLines)
    {
        for (int y = line; y > 0; --y)
        {
            for (int x = 0; x < Width; ++x)
            {
                data[x][y] = data[x][y - 1];
                dataTypes[x][y] = dataTypes[x][y - 1];
            }
        }
        for (int x = 0; x < Width; ++x)
        {
            data[x][0] = false;
            dataTypes[x][0] = Tetromino::Type::I; // 초기값 설정
        }
    }
}

int Well::getLine() const
{
    return line;
}

int Well::getTetris() const
{
    return tetris;
}
