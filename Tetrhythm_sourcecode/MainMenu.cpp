#include "MainMenu.h"
#include "Characters.h"
#include "TetrisScene.h"


MainMenu::MainMenu(WindowManager& wm, SceneManager& manager) : windowManager(wm), sceneManager(manager), print(new Print(&wm))
{
    drawInit();
}

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

        switch (MenuSelection)
        {
        
        case 0 : // 싱글 플레이 모드 씬 전환
            sceneManager.changeScene(std::make_unique<TetrisScene>(windowManager, sceneManager));
            break;
        case 1 : // muti mode
            break;
        case 2: //캐릭터 선택
            sceneManager.changeScene(std::make_unique<Characters>(windowManager, sceneManager));
            break;
      
        default:
            break;
        }

        break;
            
        break;
    }
}

void MainMenu::drawSelection() {
    int posX = 407;
    int posY = 328 + (MenuSelection * 94); // 선택된 항목에 따라 Y 위치 조정
    Print* pt = print;
    // Delete and print the PNG at the calculated position
    pt->deletePNG("Selection2.png");
    pt->printPNG("Selection2.png", posX, posY, 3);
}

void MainMenu::drawInit() {
    Print* pt = print;
    pt->printPNG("BackGround.png", 0, 0, 0);
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


void MainMenu::handleEvents() {
    print->handleEvents([&](SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            handleArrowKey(event.key.keysym.sym);
        }
    });
}

void MainMenu::update() {
    // 업데이트 로직 추가

}

void MainMenu::render() {
    drawSelection();
    print->updateAnimations();
    print->render();
}

// MAIN.cpp 파일로 옮김. 필요없으면 지워 주세요! 
//int main(int argc, char* argv[]) {
//    // 프레임 속도: 60
//    const int targetFPS = 60;
//    const std::chrono::milliseconds frameDuration(1000 / targetFPS);
//    WindowManager windowManager("Tetrhythm", 1200, 675);
//
//    Print* print = new Print(&windowManager);
//    MainMenu mn ;
//    mn.drawInit(print);
//
//    while (true) {
//        print->handleEvents([&](SDL_Event& event) {
//            if (event.type == SDL_KEYDOWN) {
//                mn.handleArrowKey(event.key.keysym.sym);
//            }
//            });
//        mn.drawSelection(print);
//
//        print->updateAnimations();
//        print->render();
//
//        std::this_thread::sleep_for(frameDuration);
//    }
//
//    return 0;
//}