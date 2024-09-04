#include "joinRoomScene.h"
#include "MultiGameScene.h"


joinRoomScene::joinRoomScene(WindowManager& wm, SceneManager& manager)
//객체 생성 및 연결
    : windowManager(wm), sceneManager(manager), print(new Print(&wm))
{
    drawInit();
}


void joinRoomScene::drawInit()
{
    //객체 연결
    Print* pt = print;
    //프린트 세팅
    print->printPNG("background.png", 0, 0, 0);
    print->printPNG("IDInputBox.png", 375, 337.5, 1);
    print->printPNG("InputCode.png", 364, 186, 2);
    print->printPNG("ToCompleteSpace.png", 908.25, 582, 3);

    //input text 세팅
    TTF_Font* font = print->loadFont("DungGeunMo.ttf", 30);
    SDL_Color color = { 255, 255, 255 }; // 흰색
    print->InputText(390, 360, 4, font, color);
    SDL_StartTextInput();
    
}


void joinRoomThread(std::string pwd) {
    try {

        Multi::getInstance()->joinRoom(pwd);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught in thread: " << e.what() << std::endl;
    }
}


void joinRoomScene::whenSpace()
{
    const std::string pwd = print->getTextInput().c_str();
    try {
      std::thread joinRoomThreadObj(joinRoomThread, pwd);
     // 스레드를 detach 또는 join 하여 관리
      joinRoomThreadObj.detach();
    }
    catch (const std::exception& e) {

      std::cerr << "Exception caught: " << e.what() << std::endl;
    }

}

void joinRoomScene::handleArrowKey(SDL_Keycode key) {
    switch (key) {

    case SDLK_ESCAPE:
        Multi::getInstance()->isWaiting = false;
        Multi::getInstance()->closeConnection();
        sceneManager.goBack();
    case SDLK_SPACE:
        whenSpace();

        break;
    }
}

void joinRoomScene::update()
{
    if (print->getTextInput().size() == 0) {
        print->updateAnimations();
    }


    if (Multi::getInstance()->isReady) {
        sceneManager.changeScene(std::make_unique<MultiGameScene>(windowManager, sceneManager));
    }
}

void joinRoomScene::render()
{
    print->renderWithTextInputAndPNG();
}


//void joinRoomScene::handleEvents() {
//    print->handleEvents([&](SDL_Event& event) {
//        if (event.type == SDL_KEYDOWN) {
//            handleArrowKey(event.key.keysym.sym);
//        }
//        });
//}
//

void joinRoomScene::handleEvents()
{
    //[this] : 현재 객체의 포인터를 캡처합니다.이를 통해 람다 함수 내부에서 클래스의 멤버 함수와 멤버 변수에 접근할 수 있습니다.
    //    () : 람다 함수가 인자를 받지 않음을 의미합니다.
    //{ this->whenEnter(); } : 람다 함수가 호출될 때 this->whenEnter()를 실행합니다.이는 현재 객체의 whenEnter 멤버 함수를 호출하는 것을 의미합니다.
    print->handleTextEvents([this]() { this->whenSpace(); }, [&](SDL_Event& event) {
       // handleClosePopup(event.key.keysym.sym);
        if (event.key.keysym.sym == SDLK_ESCAPE ) {
            sceneManager.goBack();
        }
 });
}