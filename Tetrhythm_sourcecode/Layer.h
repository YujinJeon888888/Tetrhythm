#ifndef LAYER_H
#define LAYER_H

#include <SDL.h>
#include <string>

// Layer 클래스는 하나의 이미지 레이어를 나타냅니다.
// 주어진 경로의 이미지를 로드하고 이를 SDL_Texture로 변환하여 저장합니다.
class Layer {
public:
    // 생성자: 이미지 파일 경로와 SDL_Renderer를 받아 이미지를 로드합니다.
    Layer(const std::string& imagePath, SDL_Renderer* renderer);

    // 소멸자: 텍스처를 해제합니다.
    ~Layer();

    // render: SDL_Renderer에 텍스처를 그립니다.
    void render(SDL_Renderer* renderer);

private:
    SDL_Texture* texture = nullptr; // 이미지를 렌더링하기 위한 SDL 텍스처
};

#endif // LAYER_H
