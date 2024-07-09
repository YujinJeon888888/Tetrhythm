#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <vector>
#include <string>
#include "Layer.h"

// Renderer 클래스는 여러 레이어를 관리하고 화면에 그리는 역할을 합니다.
// SDL을 초기화하고 창과 렌더러를 생성하며, 레이어를 추가하고 화면에 그립니다.
class Renderer {
public:
    // 생성자: 창의 너비와 높이를 받아 SDL을 초기화하고 창과 렌더러를 생성합니다.
    Renderer(int width, int height);

    // 소멸자: 모든 레이어와 SDL 리소스를 해제합니다.
    ~Renderer();

    // addLayer: 새로운 레이어를 추가합니다.
    void addLayer(const std::string& imagePath);

    // render: 모든 레이어를 순서대로 화면에 그립니다.
    void render();

    // run: 메인 루프를 실행하여 이벤트를 처리하고 화면을 갱신합니다.
    void run();

private:
    SDL_Window* window = nullptr; // SDL 창
    SDL_Renderer* renderer = nullptr; // SDL 렌더러
    std::vector<Layer*> layers; // 레이어 목록
};

#endif // RENDERER_H
