#include "MutiMenuScene.h"


MutiMenuScene::MutiMenuScene(WindowManager& wm, SceneManager& manager) 
	: windowManager(wm), sceneManager(manager), print(new Print(&wm))
{
	drawInit();
}



void MutiMenuScene::handleArrowKey(SDL_Keycode key) {
    switch (key) {

    case SDLK_ESCAPE : 
        sceneManager.goBack();
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

        case 0: // random mode
            break;
        case 1: // make room
            break;
        case 2: //enter room code 
          //  sceneManager.changeScene(std::make_unique<Characters>(windowManager, sceneManager));
            break;

        default:
            break;
        }

        break;

        break;
    }
}

void MutiMenuScene::drawSelection() {
    int posX = 407;
    int posY = 328 + (MenuSelection * 94); // 선택된 항목에 따라 Y 위치 조정
    Print* pt = print;
    // Delete and print the PNG at the calculated position
    pt->deletePNG("Selection2.png");
    pt->printPNG("Selection2.png", posX, posY, 3);
}

void MutiMenuScene::drawInit() {
    Print* pt = print;
    pt->printPNG("BackGround.png", 0, 0, 0);
    pt->printPNG("Group 105.png", 92, 92, 1);
    pt->printPNG("Line 22.png", 600, 0, 3);
    pt->printPNG("image 38.png", 819, 381, 2);
    pt->printPNG("image 39.png", 217, 381, 2);
    pt->printPNG("image 40.png", 789, 459, 2);

    // 초기 선택 상태 설정
    int posX = 170;
    int posY = 388;
    pt->printPNG("Selection2.png", posX, posY, 3);

  
}


void MutiMenuScene::handleEvents() {
    print->handleEvents([&](SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            handleArrowKey(event.key.keysym.sym);
        }
        });
}

void MutiMenuScene::update() {
    // 업데이트 로직 추가

}

void MutiMenuScene::render() {
    drawSelection();
    print->updateAnimations();
    print->render();
}