#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <vector>
#include <string>
#include "Layer.h"
#include "Animation.h"
#include "LayerType.h"

// Renderer Ŭ������ ���� ���̾ �����ϰ� ȭ�鿡 �׸��� ������ �մϴ�.
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
