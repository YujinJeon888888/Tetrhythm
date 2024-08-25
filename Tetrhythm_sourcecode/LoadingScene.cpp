#include "LoadingScene.h"
#include "MultiGameScene.h"


LoadingScene::LoadingScene(WindowManager& wm, SceneManager& manager)
	: windowManager(wm), sceneManager(manager), print(new Print(&wm))
{
	drawInit();

}

void LoadingScene::handleArrowKey(SDL_Keycode key) {
    switch (key) {

    case SDLK_ESCAPE:
        Multi::getInstance()->closeConnection();
        Multi::resetInstance();
        sceneManager.goBack();
    case SDLK_UP:
     
        break;
    case SDLK_DOWN:
       
        break;
    case SDLK_RIGHT:
     
        break;
    case SDLK_LEFT:

    

        break;
    case SDLK_SPACE:
        

        break;
    }
}

void LoadingScene::drawInit() {

    print->printPNG("BackGround.png", 0, 0, 10);
    std::vector<std::string> animPaths1 = { "Loading1.png","Loading2.png","Loading3.png" };

    // 현재 인덱스의 이미지를 출력
    print->printAnimationPNG(animPaths1, 917, 572, 11, 60);

}



void LoadingScene::handleEvents() {
    print->handleEvents([&](SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            handleArrowKey(event.key.keysym.sym);
        }
        });
}

void LoadingScene::update() {
    // 업데이트 로직 추가

    if (Multi::getInstance()->isReady) {
        sceneManager.changeScene(std::make_unique<MultiGameScene>(windowManager, sceneManager));
    }

}

void LoadingScene::render() {


    print->updateAnimations();
    print->render();
}
