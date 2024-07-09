#include "Renderer.h"
#include <SDL_image.h>
#include <iostream>

// ������: â�� �ʺ�� ���̸� �޾� SDL�� �ʱ�ȭ�ϰ� â�� �������� �����մϴ�.
Renderer::Renderer(int width, int height) {
    // SDL �ʱ�ȭ
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }
    // â ����
    window = SDL_CreateWindow("SDL Layered Renderer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }
    // ������ ����
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    // SDL_image �ʱ�ȭ
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
}

// �Ҹ���: ��� ���̾�� SDL ���ҽ��� �����մϴ�.
Renderer::~Renderer() {
    // ��� ���̾� ����
    for (auto layer : layers) {
        delete layer;
    }
    // SDL ���ҽ� ����
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    IMG_Quit();
    SDL_Quit();
}

// addLayer: ���ο� ���̾ �߰��մϴ�.
void Renderer::addLayer(const std::string& imagePath) {
    layers.push_back(new Layer(imagePath, renderer));
}

// render: ��� ���̾ ������� ȭ�鿡 �׸��ϴ�.
void Renderer::render() {
    // ȭ�� �ʱ�ȭ
    SDL_RenderClear(renderer);
    // ���̾� ������� �׸���
    for (auto layer : layers) {
        layer->render(renderer);
    }
    // ȭ�� ����
    SDL_RenderPresent(renderer);
}

// run: ���� ������ �����Ͽ� �̺�Ʈ�� ó���ϰ� ȭ���� �����մϴ�.
void Renderer::run() {
    bool running = true;
    SDL_Event event;

    while (running) {
        // �̺�Ʈ ó��
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        // ȭ�� �׸���
        render();
        SDL_Delay(16); // �� 60 FPS
    }
}
