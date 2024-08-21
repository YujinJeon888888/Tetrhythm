#include "Well.h"
#include "Print.h"

// 각 블럭의 크기
const int BLOCK_SIZE = 25;
const int QUEUE_BLOCK_SIZE = 20; // 대기열 블럭의 크기

Well::Well(int xOffset, int yOffset, int queueXOffset, int queueYOffset) :
    xOffset(xOffset),  // 전달된 매개변수를 사용
    yOffset(yOffset),
    queueXOffset(queueXOffset),  // 전달된 매개변수를 사용
    queueYOffset(queueYOffset),
    line(0),
    tetris(0)
{
    for (int x = 0; x < Width; ++x)
        for (int y = 0; y < Height; ++y) {
            data[x][y] = false;
            dataTypes[x][y] = Tetromino::Type::I; // 초기값
        }
}

//싱글모드용 draw코드
void Well::draw(SDL_Renderer* renderer, SDL_Texture* blockTextures[], const std::array<Tetromino, 3>& nextTetrominos)
{
    for (int x = 0; x < Width; ++x)
        for (int y = 0; y < Height; ++y)
        {
            if (data[x][y])
            {
                SDL_Rect rect{ xOffset + x * BLOCK_SIZE + 1, yOffset + y * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2 };
                SDL_RenderCopy(renderer, blockTextures[dataTypes[x][y]], nullptr, &rect);
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

    //블럭대기열 그리기
    SDL_Rect outerRect = { queueXOffset, queueYOffset, 90, 183 }; // queueXOffset, queueYOffset 사용
    SDL_Rect innerRect = { queueXOffset + 3, queueYOffset + 3, 84, 177 }; // queueXOffset, queueYOffset 사용
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderFillRect(renderer, &outerRect);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff); // Black background for queue box
    SDL_RenderFillRect(renderer, &innerRect);

    // Draw custom bottom border
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff); // White color for custom border



    // 블럭 대기열 내부 블럭들 그리기
    for (int i = 0; i < 3; ++i)
    {
        int boxXOffset = queueXOffset + 14; // 기존 12에서 2픽셀 오른쪽으로 이동
        int boxYOffset = queueYOffset + i * 60 + 10;

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
            offsetX = -0.5; offsetY = 0.5; // I 블럭을 중앙에 위치시키기 위한 조정
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

//멀티모드용 draw코드 (회색 텍스처 추가)
void Well::draw(SDL_Renderer* renderer, SDL_Texture* blockTextures[], SDL_Texture* grayBlockTexture, const std::array<Tetromino, 3>& nextTetrominos)
{
    for (int x = 0; x < Width; ++x)
    {
        for (int y = 0; y < Height; ++y)
        {
            if (data[x][y])
            {
                SDL_Rect rect{ xOffset + x * BLOCK_SIZE + 1, yOffset + y * BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2 };
                SDL_Texture* texture = (dataTypes[x][y] == Tetromino::Type::GRAY) ? grayBlockTexture : blockTextures[dataTypes[x][y]];
                SDL_RenderCopy(renderer, texture, nullptr, &rect);
            }
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

    //블럭대기열 그리기
    SDL_Rect outerRect = { queueXOffset, queueYOffset, 90, 183 }; // queueXOffset, queueYOffset 사용
    SDL_Rect innerRect = { queueXOffset + 3, queueYOffset + 3, 84, 177 }; // queueXOffset, queueYOffset 사용
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderFillRect(renderer, &outerRect);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff); // Black background for queue box
    SDL_RenderFillRect(renderer, &innerRect);

    // Draw custom bottom border
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff); // White color for custom border
    


    // 블럭 대기열 내부 블럭들 그리기
    for (int i = 0; i < 3; ++i)
    {
        int boxXOffset = queueXOffset + 14; // 기존 12에서 2픽셀 오른쪽으로 이동
        int boxYOffset = queueYOffset + i * 60 + 10;

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
            offsetX = -0.5; offsetY = 0.5; // I 블럭을 중앙에 위치시키기 위한 조정
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


void Well::addGrayLines(int numLines, bool Gap)
{
    // 빈 칸 위치를 미리 결정
    int gapX = Gap ? rand() % Width : -1;

    // 기존 블럭들을 위로 이동
    for (int y = 0; y < Height - numLines; ++y)
    {
        for (int x = 0; x < Width; ++x)
        {
            data[x][y] = data[x][y + numLines];
            dataTypes[x][y] = dataTypes[x][y + numLines];
        }
    }

    // 아래에 회색 블럭 추가
    for (int y = Height - numLines; y < Height; ++y)
    {
        for (int x = 0; x < Width; ++x)
        {
            if (x != gapX)
            {
                data[x][y] = true;
                dataTypes[x][y] = Tetromino::Type::GRAY; // 새로운 회색 블럭 타입
            }
            else
            {
                data[x][y] = false;
                dataTypes[x][y] = Tetromino::Type::I; // 빈칸은 텍스처 없음
            }
        }
    }
}

