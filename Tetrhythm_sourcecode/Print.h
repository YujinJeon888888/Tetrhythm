#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <random>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include "WindowManager.h"
#include <SDL_ttf.h> // 폰트출력추가
#include "MySQL.h"
struct TextAnimation {
    std::string text;
    std::vector<SDL_Color> colors;
    int currentFrame;
    int frameCount;
    int frameDelay;
    int layer;
    SDL_Rect textRect;
    TTF_Font* font;
};

struct FontInfo {
    TTF_Font* font;
    SDL_Color color;
    int layer;
    SDL_Rect dst;
    SDL_Texture* texture;
};


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
    std::vector<FontInfo> fontInfos;
    std::string textInput = "";
    FontInfo textInputObj;
    std::vector<TextAnimation> textAnimations;
public:
    Print(WindowManager* wm);
    ~Print();
    void handleEvents(const std::function<void(SDL_Event&)>& onEvent);
    void handleEvents();
    void handleTextEvents(const std::function<void()>& whenSpace, const std::function<void(SDL_Event&)>& onEvent = nullptr);
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
    void printText(const std::string& text, const int& dstX, const int& dstY, int layer, TTF_Font* font, SDL_Color color); // 폰트출력추가
    void InputText(const int& dstX, const int& dstY, int layer, TTF_Font* font, SDL_Color color);
    void setText(int layer, const std::string& newText);
    TTF_Font* loadFont(const char* path, int size); // 폰트출력추가
    void unloadFont(TTF_Font* font); // 폰트출력추가
    void renderWithTextInputAndPNG();
    std::vector<LayeredTexture> getLayeredTextures();
    std::string getTextInput();
    void renderForTetris();
    void textAnimation(const std::string& text, const int& dstX, const int& dstY, TTF_Font* font, const std::vector<SDL_Color>& colors, int frameDelay, int layer);
    void updateTextAnimation();


};
