#include "Game.h"
#include <stdexcept>
#include <iostream>
#include "tetromino.h"
#include "Well.h"
#include "Print.h"
#include "WindowManager.h"
#include <vector>
#include <thread>
#include <chrono>

Game::Game(WindowManager& wm, Print* pr)
    :
    tetromino_{ static_cast<Tetromino::Type>(rand() % 7) },
    moveTime_(SDL_GetTicks()),
    previousLine(0),
    previousTetris(0),
    gameOver(false),
    windowManager(wm),
    print(pr)
{
    // 7개의 텍스처 로드
    const char* textureFiles[7] = {
        "Skyblue_I.png",
        "Blue_J.png",
        "Orange_L.png",
        "Yellow_O.png",
        "Green_S.png",
        "Purple_T.png",
        "Red_Z.png"
    };

    for (int i = 0; i < 7; ++i)
    {
        SDL_Surface* surface = IMG_Load(textureFiles[i]);
        if (!surface)
        {
            throw std::runtime_error("Failed to load image: " + std::string(IMG_GetError()));
        }
        blockTextures_[i] = SDL_CreateTextureFromSurface(windowManager.getRenderer(), surface);
        SDL_FreeSurface(surface);
        if (!blockTextures_[i])
        {
            throw std::runtime_error("Failed to create texture: " + std::string(SDL_GetError()));
        }
    }
}

Game::~Game()
{
    for (int i = 0; i < 7; ++i)
    {
        if (blockTextures_[i])
        {
            SDL_DestroyTexture(blockTextures_[i]);
        }
    }
}

bool Game::tick()
{
    if (gameOver)
        return false;

    SDL_Event e;
    if (SDL_WaitEventTimeout(&e, 250))
    {
        switch (e.type)
        {
        case SDL_KEYDOWN:
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_DOWN:
            {
                Tetromino t = tetromino_;
                t.move(0, 1);
                if (!well_.isCollision(t))
                    tetromino_ = t;
            }
            break;
            case SDLK_RIGHT:
            {
                Tetromino t = tetromino_;
                t.move(1, 0);
                if (!well_.isCollision(t))
                    tetromino_ = t;
            }
            break;
            case SDLK_LEFT:
            {
                Tetromino t = tetromino_;
                t.move(-1, 0);
                if (!well_.isCollision(t))
                    tetromino_ = t;
            }
            break;
            case SDLK_UP:
            {
                Tetromino t = tetromino_;
                t.rotate();
                if (!well_.isCollision(t))
                    tetromino_ = t;
            }
            break;
            case SDLK_c:
            {
                Tetromino t = tetromino_;
                t.rotate();
                if (!well_.isCollision(t))
                    tetromino_ = t;
            }
            break;
            case SDLK_z:
            {
                Tetromino t = tetromino_;
                t.rotateCounterClockwise();
                if (!well_.isCollision(t))
                    tetromino_ = t;
            }
            break;
            case SDLK_SPACE:
            {
                Tetromino t = tetromino_;
                t.drop(well_);
                check(t);
            }
            break;
            }
        }
        break;
        case SDL_QUIT:
            return false;
        }
    }
    SDL_SetRenderDrawColor(windowManager.getRenderer(), 0, 0, 0, 0xff);
    //화면업데이트
    SDL_RenderClear(windowManager.getRenderer());
    well_.draw(windowManager.getRenderer(), blockTextures_);
    tetromino_.draw(windowManager.getRenderer(), blockTextures_[tetromino_.getType()]);
    print->printPNGForTetris("CharacterSize.png", 0, 0, 1);

    if (SDL_GetTicks() > moveTime_)
    {
        moveTime_ += 1000;
        Tetromino t = tetromino_;
        t.move(0, 1);
        check(t);
    }
    //화면업데이트
    SDL_RenderPresent(windowManager.getRenderer());

    int currentLine = well_.getLine();
    int currentTetris = well_.getTetris();

    if (currentLine > previousLine)
    {
        std::cout << "Line: " << currentLine << std::endl;
        previousLine = currentLine;
    }

    if (currentTetris > previousTetris)
    {
        std::cout << "Tetris: " << currentTetris << std::endl;
        previousTetris = currentTetris;
    }

    return true;
}

void Game::check(const Tetromino& t)
{
    if (well_.isCollision(t))
    {
        well_.unite(tetromino_);
        tetromino_ = Tetromino{ static_cast<Tetromino::Type>(rand() % 7) };
        if (well_.isCollision(tetromino_))
        {
            gameOver = true;
            std::cout << "Game Over!" << std::endl;
        }
    }
    else
    {
        tetromino_ = t;
    }
}

bool Game::isGameOver() const
{
    return gameOver;
}

SDL_Texture* Game::getBlockTexture(Tetromino::Type type) const
{
    return blockTextures_[type];
}
