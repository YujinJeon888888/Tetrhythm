#define WIN32_LEAN_AND_MEAN
#include "scoreTableScene.h"
#include "MutiMenuScene.h"
#include "MultiGameScene.h"
#include "MainMenu.h"
#include "Characters.h"
#include "SceneManager.h"
#include "TetrisScene.h"
#include "ControlExp.h"


MainMenu::MainMenu(WindowManager& wm, SceneManager& manager) : windowManager(wm), sceneManager(manager), print(new Print(&wm)), soundManager(new SoundManager())
{
    drawInit();
}

void MainMenu::handleArrowKey(SDL_Keycode key) {
    switch (key) {
    case SDLK_LSHIFT:
        soundManager->loadSound("Musics/Selection.mp3", "Selection"); // 효과음 로드
        soundManager->playSound("Selection", 0);
        sceneManager.changeScene(std::make_unique<ControlExp>(windowManager, sceneManager));
        break;
    case SDLK_ESCAPE:
        soundManager->loadSound("Musics/Selection.mp3", "Selection"); // 효과음 로드
        soundManager->playSound("Selection", 0);
        sceneManager.changeScene(std::make_unique<scoreTableScene>(windowManager, sceneManager));
        break;
    case SDLK_UP:
        soundManager->loadSound("Musics/SelectionMove.mp3", "SelectionMove"); // 효과음 로드
        soundManager->playSound("SelectionMove", 0);
        if (MenuSelection > 0) {
            MenuSelection--;
        }
        break;
    case SDLK_DOWN:
        soundManager->loadSound("Musics/SelectionMove.mp3", "SelectionMove"); // 효과음 로드
        soundManager->playSound("SelectionMove", 0);
        if (MenuSelection < numOptions - 1) {
            MenuSelection++;
        }
        break;
    case SDLK_SPACE:
    case SDLK_RETURN:
        soundManager->loadSound("Musics/Selection.mp3", "Selection"); // 효과음 로드
        soundManager->playSound("Selection", 0);
        std::cout << "Selected Menu: " << MenuSelection << std::endl;

        switch (MenuSelection)
        {
        
        case 0 : // 싱글 플레이 모드 씬 전환
            sceneManager.changeScene(std::make_unique<TetrisScene>(windowManager, sceneManager));
            break;
        case 1 : // muti mode
            /*sceneManager.changeScene(std::make_unique<MutiMenuScene>(windowManager, sceneManager));*/
            sceneManager.changeScene(std::make_unique<MutiMenuScene>(windowManager, sceneManager));
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
    //pt->printPNG("PerTitle.png", 300, 102.75, 2);
    pt->printPNG("Shift_1.png", 930.75, 546.5, 1);
    pt->printPNG("title.png", 214, 29, 2);
    pt->printPNG("MainExplain.png", 966.75, 586.5, 3);
    pt->printPNG("main_esc.png", 65, 586.5, 3);

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
