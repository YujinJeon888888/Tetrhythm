#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL.h>
#include <vector>
#include <string>
#include "LayerType.h"

// Animation Ŭ������ ���� �ð����� ���� ������� �Ѿ�� �ִϸ��̼��� ��Ÿ���ϴ�.
class Animation {
public:
    // ������: �̹��� ���� ��� ����Ʈ, SDL_Renderer, ��ġ, z-index, ���̾� Ÿ��, ������ �� �ð� ������ �޽��ϴ�.
    Animation(const std::vector<std::string>& imagePaths, SDL_Renderer* renderer, int x, int y, int zIndex, LayerType type, int frameDelay);
    ~Animation();
    void render(SDL_Renderer* renderer);
    int getZIndex() const;
    LayerType getType() const;

private:
    std::vector<SDL_Texture*> textures; // �ִϸ��̼� ������ �ؽ�ó ���
    SDL_Rect dstRect; // �ؽ�ó�� ��ġ�� ũ�⸦ �����ϴ� �簢��
    int zIndex; // ���̾��� z-index
    LayerType type; // ���̾� Ÿ��
    int frameDelay; // ������ �� �ð� ���� (�и���)
    size_t currentFrame; // ���� ������ �ε���
    Uint32 lastFrameTime; // ������ �������� ����� �ð�
};

#endif // ANIMATION_H
