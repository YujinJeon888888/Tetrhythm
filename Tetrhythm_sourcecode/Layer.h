#ifndef LAYER_H
#define LAYER_H

#include <SDL.h>
#include <string>
#include "LayerType.h"

// Layer 클래스는 하나의 이미지 레이어를 나타냅니다.
class Layer {
public:
    Layer(const std::string& imagePath, SDL_Renderer* renderer, int x, int y, int zIndex, LayerType type);
    ~Layer();
    void render(SDL_Renderer* renderer);
    int getZIndex() const;
    LayerType getType() const;

private:
    SDL_Texture* texture = nullptr;
    SDL_Rect dstRect;
    int zIndex;
    LayerType type;
};

#endif // LAYER_H
