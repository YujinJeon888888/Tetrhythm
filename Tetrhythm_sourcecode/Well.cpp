#include "Well.h"
#include "Print.h"

// 각 블럭의 크기
const int BLOCK_SIZE = 25;
const int QUEUE_BLOCK_SIZE = 20; // 대기열 블럭의 크기

Well::Well() :
    xOffset(513),
    yOffset(116),
    line(0),
    tetris(0)
{
    for (int x = 0; x < Width; ++x)
        for (int y = 0; y < Height; ++y) {
            data[x][y] = false;
            dataTypes[x][y] = Tetromino::Type::I; // 초기값
        }
}

void Well::draw(SDL_Renderer* renderer, SDL_Texture* blockTextures[], const std::array<Tetromino, 3>& nextTetrominos)
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

    // Draw game over line after blocks
    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0x33); // Red color with 20% transparency
    int lineStart = xOffset + 5; // 엔드라인 시작 지점 (조금 안쪽으로)
    int lineEnd = xOffset + wellWidth - 10; // 엔드라인 끝 지점 (조금 안쪽으로)
    SDL_RenderDrawLine(renderer, lineStart, yOffset, lineEnd, yOffset);

    // Draw next tetromino queue box with 3px border
    SDL_Rect outerRect = { 806, 130, 90, 183 }; // Height increased by 3px
    SDL_Rect innerRect = { 809, 133, 84, 177 }; // Adjusted for 3px border
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderFillRect(renderer, &outerRect);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff); // Black background for queue box
    SDL_RenderFillRect(renderer, &innerRect);

    // Draw custom bottom border
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff); // White color for custom border
    SDL_RenderDrawLine(renderer, 806, 310, 896, 310); // Bottom border with 3px offset


    // Draw the next tetrominos
    for (int i = 0; i < 3; ++i)
    {
        int boxXOffset = 809 + 12; // 상자 내부의 중앙으로 조정
        int boxYOffset = 133 + i * 60 + 10; // 상자 내부의 중앙으로 조정

        // Adjust rotation for tetrominos to fit within the box
        Tetromino adjustedTetromino = nextTetrominos[i];

        // Rotate tetromino if needed to fit within 90x60 box
        if (adjustedTetromino.getType() != Tetromino::Type::O) {
            adjustedTetromino.rotate();
        }

        // Adjust the block positions to fit within 90x60 box
        int minX = 4, minY = 4, maxX = 0, maxY = 0;
        for (auto x = 0; x < 4; ++x)
            for (auto y = 0; y < 4; ++y)
                if (adjustedTetromino.isBlock(x, y))
                {
                    if (x < minX) minX = x;
                    if (y < minY) minY = y;
                    if (x > maxX) maxX = x;
                    if (y > maxY) maxY = y;
                }

        double offsetX = (4 - (maxX - minX + 1)) / 2;
        double offsetY = (3 - (maxY - minY + 1)) / 2;

        if (adjustedTetromino.getType() == Tetromino::Type::I) {
            offsetX = -0.5;  offsetY = 0.5; // I 블럭을 중앙에 위치시키기 위한 조정
        }
        else if (adjustedTetromino.getType() == Tetromino::Type::O) {
            offsetX = 0.5;
        } // O 블럭을 중앙에 위치시키기 위한 조정 

        for (auto x = 0; x < 4; ++x)
            for (auto y = 0; y < 4; ++y)
                if (adjustedTetromino.isBlock(x, y))
                {
                    SDL_Rect rect{ boxXOffset + (x - minX + offsetX) * QUEUE_BLOCK_SIZE + 1, boxYOffset + (y - minY + offsetY) * QUEUE_BLOCK_SIZE + 1, QUEUE_BLOCK_SIZE - 2, QUEUE_BLOCK_SIZE - 2 };
                    SDL_RenderCopy(renderer, blockTextures[adjustedTetromino.getType()], nullptr, &rect);
                }
    }
}

void Well::drawShadow(SDL_Renderer* renderer, SDL_Texture* blockTexture, const Tetromino& shadow) const
{
    SDL_SetTextureAlphaMod(blockTexture, 128); // 블럭 텍스처의 알파 값을 조절하여 반투명하게 설정합니다.
    shadow.draw(renderer, blockTexture);
    SDL_SetTextureAlphaMod(blockTexture, 255); // 블럭 텍스처의 알파 값을 원래대로 복원합니다.
}

bool Well::isCollision(const Tetromino& t) const
{
    for (int x = 0; x < 4; ++x)
        for (int y = 0; y < 4; ++y)
            if (t.isBlock(x, y))
            {
                auto wx = t.x() + x;
                auto wy = t.y() + y;
                if (wx < 0 || wx >= Width || wy >= Height) // wy < 0 을 삭제하여 well 상단 영역에서의 충돌을 무시
                    return true;
                if (wy >= 0 && data[wx][wy]) // 블럭이 Well 내부에 위치할 때만 충돌 검사
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
