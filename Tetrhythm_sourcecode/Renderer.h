#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <vector>
#include <string>
#include "Layer.h"
#include "Animation.h"
#include "LayerType.h"

// Renderer 클래스는 여러 레이어를 관리하고 화면에 그리는 역할을 합니다.
class Renderer {
public:
    Renderer(int width, int height);
    ~Renderer();
    void addLayer(const std::string& imagePath, int x, int y, int zIndex, LayerType type);
    void addAnimation(const std::vector<std::string>& imagePaths, int x, int y, int zIndex, LayerType type, int frameDelay);
    void render();
    void run();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::vector<Layer*> layers;
    std::vector<Animation*> animations;
    const int targetFPS = 60;
    const int frameDelay = 1000 / targetFPS;
};

#endif // RENDERER_H
