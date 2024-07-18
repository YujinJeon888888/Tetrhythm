#include "Print.h"
#include "WindowManager.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class MainMenu {
public:
    MainMenu();
    void drawInit(Print* renderer);
    void drawSelection(Print* renderer);
    void handleArrowKey(SDL_Keycode key);

private:
    static const int numOptions = 3;
    int MenuSelection = 0;
};

MainMenu::MainMenu() {}

void MainMenu::handleArrowKey(SDL_Keycode key) {
    switch (key) {
    case SDLK_UP:
        if (MenuSelection > 0) {
            MenuSelection--;
        }
        break;
    case SDLK_DOWN:
        if (MenuSelection < numOptions - 1) {
            MenuSelection++;
        }
        break;
    case SDLK_SPACE:
        std::cout << "Selected Menu: " << MenuSelection << std::endl;
        break;
    }
}

void MainMenu::drawSelection(Print* pt) {
    int posX = 407;
    int posY = 328 + (MenuSelection * 94); // 선택된 항목에 따라 Y 위치 조정

    // Delete and print the PNG at the calculated position
    pt->deletePNG("Selection2.png");
    pt->printPNG("Selection2.png", posX, posY, 3);
}

void MainMenu::drawInit(Print* pt) {
    pt->printPNG("PerTitle.png", 300, 102.75, 2);
    pt->printPNG("MainExplain.png", 966.75, 586.5, 3);

    // 초기 선택 상태 설정
    int posX = 407;
    int posY = 328;
    pt->printPNG("Selection2.png", posX, posY, 3);

    // 메뉴 항목들 그리기
    pt->printPNG("SinglePlay.png", 462, 315, 2); // 싱글 플레이
    pt->printPNG("MultiPlay.png", 462, 315 + 94, 2); // 멀티 플레이
    pt->printPNG("MyCharacter.png", 462, 315 + 188, 2); // 나의 캐릭터
}

int main(int argc, char* argv[]) {
    // 프레임 속도: 60
    const int targetFPS = 60;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    WindowManager windowManager("Tetrhythm", 1200, 675);

    Print* print = new Print(&windowManager);
    MainMenu mn;
    mn.drawInit(print);

    while (true) {
        print->handleEvents([&](SDL_Event& event) {
            if (event.type == SDL_KEYDOWN) {
                mn.handleArrowKey(event.key.keysym.sym);
            }
            });
        mn.drawSelection(print);

        print->updateAnimations();
        print->render();

        std::this_thread::sleep_for(frameDuration);
    }

    return 0;
}