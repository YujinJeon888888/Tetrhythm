#include "SDL.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // SDL�� �ʱ�ȭ�Ǿ����� Ȯ���ϱ� ���� �޽��� �ڽ��� ���ϴ�.
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
        "SDL Initialization",
        "SDL has been initialized successfully!",
        NULL);
    SDL_Quit();
    return 0;
}
