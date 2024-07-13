#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "WindowManager.h"

struct LayeredTexture {
    SDL_Texture* texture;
    SDL_Rect dstRect;
    int layer;
    std::string path;
};

struct Animation {
    std::vector<std::string> paths;
    int currentFrame;
    int frameCount;
    int frameDelay;
    int layer;
    SDL_Rect animRect;
};

class Print {
private:
    WindowManager* windowManager;
    SDL_Renderer* renderer;
    SDL_Event event;
    std::vector<LayeredTexture> layeredTextures;
    std::vector<Animation> animations;
    SDL_Texture* createTextureFromPath(const std::string& path);

public:
    Print(WindowManager* wm);
    ~Print();
    void printPNG(const char* path, const int& dstX = 0, const int& dstY = 0, int layer = 0);
    void printAnimationPNG(const std::vector<std::string>& paths, const int& dstX = 0, const int& dstY = 0, int layer = 0, int frameDelay = 60);
    void deletePNG(const char* path);
    void deleteAnimation(const std::vector<std::string>& paths);
    void deleteLayer(int layer);
    void render();
    void handleEvents();
    void updateAnimations();
    void clearScreen();
    void moveImage(const char* path, const int& dstX, const int& dstY); // 이미지 이동 메소드 추가
    SDL_Rect getImagePosition(const char* path); // 이미지 위치 반환 메소드 추가
};
