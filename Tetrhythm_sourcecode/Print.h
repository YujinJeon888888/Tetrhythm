#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm> // <algorithm> 헤더 추가

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
    SDL_Window* window; // 앞으로 만들 윈도우(화면이 그려지는 종이)
    SDL_Renderer* renderer; // 렌더러(화면을 그리는 개체)가 들어갈 포인터변수
    SDL_Event event;
    std::vector<LayeredTexture> layeredTextures; // 여러 텍스처를 레이어 정보와 함께 저장할 벡터
    std::vector<Animation> animations; // 여러 애니메이션을 저장할 벡터

public:
    Print();
    ~Print();

    void printPNG(const char* path, const int& dstX = 0, const int& dstY = 0, int layer = 0);
    void printAnimationPNG(const std::vector<std::string>& paths, const int& dstX = 0, const int& dstY = 0, int layer = 0, int frameDelay = 60);
    void deletePNG(const char* path);
    void deleteLayer(int layer); // 특정 레이어의 모든 텍스처를 삭제하는 함수 추가
    void setLayer(const char* path, int layer);
    void render();
    void handleEvents();
    SDL_Texture* createTextureFromPath(const std::string& path);
    void updateAnimations(); // 애니메이션 업데이트 함수 추가
};
