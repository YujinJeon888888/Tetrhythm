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

class Print {
private:
    SDL_Window* window; // 앞으로 만들 윈도우(화면이 그려지는 종이)
    SDL_Renderer* renderer; // 렌더러(화면을 그리는 개체)가 들어갈 포인터변수
    SDL_Event event;
    std::vector<LayeredTexture> layeredTextures; // 여러 텍스처를 레이어 정보와 함께 저장할 벡터

public:
    Print();
    ~Print();

    void printPNG(const char* path, const int& dstX = 0, const int& dstY = 0, int layer = 0);
    void deletePNG(const char* path);
    void setLayer(const char* path, int layer);
    void run(const Uint32& ms = 16);

    // 애니메이션 PNG 출력 함수
    void printAnimationPNG(const std::vector<std::string>& paths, const int& dstX = 0, const int& dstY = 0, int layer = 0, int frameDelay = 60);
};
