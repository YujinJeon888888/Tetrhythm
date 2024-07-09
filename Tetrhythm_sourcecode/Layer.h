#ifndef LAYER_H
#define LAYER_H

#include <SDL.h>
#include <string>

// Layer Ŭ������ �ϳ��� �̹��� ���̾ ��Ÿ���ϴ�.
// �־��� ����� �̹����� �ε��ϰ� �̸� SDL_Texture�� ��ȯ�Ͽ� �����մϴ�.
class Layer {
public:
    // ������: �̹��� ���� ��ο� SDL_Renderer�� �޾� �̹����� �ε��մϴ�.
    Layer(const std::string& imagePath, SDL_Renderer* renderer);

    // �Ҹ���: �ؽ�ó�� �����մϴ�.
    ~Layer();

    // render: SDL_Renderer�� �ؽ�ó�� �׸��ϴ�.
    void render(SDL_Renderer* renderer);

private:
    SDL_Texture* texture = nullptr; // �̹����� �������ϱ� ���� SDL �ؽ�ó
};

#endif // LAYER_H
