#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
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
    void handleEvents(const std::function<void(SDL_Event&)>& onEvent);
    void handleEvents();
    void printPNG(const char* path, const int& dstX = 0, const int& dstY = 0, int layer = 0);
    void printAnimationPNG(const std::vector<std::string>& paths, const int& dstX = 0, const int& dstY = 0, int layer = 0, int frameDelay = 60);
    void deletePNG(const char* path);
    void deleteAnimation(const std::vector<std::string>& paths);
    void deleteLayer(int layer);
    void render();
    void updateAnimations();
    void clearScreen();
    void moveImage(const char* path, const int& dstX, const int& dstY); // 占싱뱄옙占쏙옙 占싱듸옙 占쌨소듸옙 占쌩곤옙
    SDL_Rect getImagePosition(const char* path); // 占싱뱄옙占쏙옙 占쏙옙치 占쏙옙환 占쌨소듸옙 占쌩곤옙
};
