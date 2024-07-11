#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm> // <algorithm> ��� �߰�

struct LayeredTexture {
    SDL_Texture* texture;
    SDL_Rect dstRect;
    int layer;
    std::string path;
};

class Print {
private:
    SDL_Window* window; // ������ ���� ������(ȭ���� �׷����� ����)
    SDL_Renderer* renderer; // ������(ȭ���� �׸��� ��ü)�� �� �����ͺ���
    SDL_Event event;
    std::vector<LayeredTexture> layeredTextures; // ���� �ؽ�ó�� ���̾� ������ �Բ� ������ ����

public:
    Print();
    ~Print();

    void printPNG(const char* path, const int& dstX = 0, const int& dstY = 0, int layer = 0);
    void deletePNG(const char* path);
    void setLayer(const char* path, int layer);
    void run(const Uint32& ms = 16);

    // �ִϸ��̼� PNG ��� �Լ�
    void printAnimationPNG(const std::vector<std::string>& paths, const int& dstX = 0, const int& dstY = 0, int layer = 0, int frameDelay = 60);
};
