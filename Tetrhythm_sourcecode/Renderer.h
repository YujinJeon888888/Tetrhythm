#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <vector>
#include <string>
#include "Layer.h"

// Renderer Ŭ������ ���� ���̾ �����ϰ� ȭ�鿡 �׸��� ������ �մϴ�.
// SDL�� �ʱ�ȭ�ϰ� â�� �������� �����ϸ�, ���̾ �߰��ϰ� ȭ�鿡 �׸��ϴ�.
class Renderer {
public:
    // ������: â�� �ʺ�� ���̸� �޾� SDL�� �ʱ�ȭ�ϰ� â�� �������� �����մϴ�.
    Renderer(int width, int height);

    // �Ҹ���: ��� ���̾�� SDL ���ҽ��� �����մϴ�.
    ~Renderer();

    // addLayer: ���ο� ���̾ �߰��մϴ�.
    void addLayer(const std::string& imagePath);

    // render: ��� ���̾ ������� ȭ�鿡 �׸��ϴ�.
    void render();

    // run: ���� ������ �����Ͽ� �̺�Ʈ�� ó���ϰ� ȭ���� �����մϴ�.
    void run();

private:
    SDL_Window* window = nullptr; // SDL â
    SDL_Renderer* renderer = nullptr; // SDL ������
    std::vector<Layer*> layers; // ���̾� ���
};

#endif // RENDERER_H
