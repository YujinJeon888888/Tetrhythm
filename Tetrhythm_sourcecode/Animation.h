#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL.h>
#include <vector>
#include <string>
#include "LayerType.h"

// Animation 클래스는 일정 시간마다 다음 장면으로 넘어가는 애니메이션을 나타냅니다.
class Animation {
public:
    // 생성자: 이미지 파일 경로 리스트, SDL_Renderer, 위치, z-index, 레이어 타입, 프레임 간 시간 간격을 받습니다.
    Animation(const std::vector<std::string>& imagePaths, SDL_Renderer* renderer, int x, int y, int zIndex, LayerType type, int frameDelay);
    ~Animation();
    void render(SDL_Renderer* renderer);
    int getZIndex() const;
    LayerType getType() const;

private:
    std::vector<SDL_Texture*> textures; // 애니메이션 프레임 텍스처 목록
    SDL_Rect dstRect; // 텍스처의 위치와 크기를 정의하는 사각형
    int zIndex; // 레이어의 z-index
    LayerType type; // 레이어 타입
    int frameDelay; // 프레임 간 시간 간격 (밀리초)
    size_t currentFrame; // 현재 프레임 인덱스
    Uint32 lastFrameTime; // 마지막 프레임이 변경된 시간
};

#endif // ANIMATION_H
