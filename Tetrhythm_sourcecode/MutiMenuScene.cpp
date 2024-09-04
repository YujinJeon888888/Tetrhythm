#include "MutiMenuScene.h"

bool isCodeScene = false;

MutiMenuScene::MutiMenuScene(WindowManager& wm, SceneManager& manager) 
	: windowManager(wm), sceneManager(manager), print(new Print(&wm)), soundManager(new SoundManager())
{
    Multi::resetInstance();
	drawInit();
    MenuSelection = 0;
}

void randomRoomThread(Multi* client) {
    try {
        client->getRandomRoom();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught in thread: " << e.what() << std::endl;
    }
}

void createRoomThread(Multi* client) {
    try {

        client->createRoom();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught in thread: " << e.what() << std::endl;
    }
}

void MutiMenuScene::handleArrowKey(SDL_Keycode key) {
    switch (key) {

    case SDLK_ESCAPE : 
        if (Multi::getInstance()->roomCode != 0) {
            Multi::getInstance()->roomCode = 0;
            deleteCode();
            isCodeScene = false;
            Multi::getInstance()->isWaiting = false;
        }
        else {
            soundManager->loadSound("Musics/Selection.mp3", "Selection"); // 효과음 로드
            soundManager->playSound("Selection", 0);
            Multi::getInstance()->isWaiting = false;
            sceneManager.goBack();
        }
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
    case SDLK_RIGHT:
        soundManager->loadSound("Musics/SelectionMove.mp3", "SelectionMove"); // 효과음 로드
        soundManager->playSound("SelectionMove", 0);
        if (MenuSelection < numOptions - 1) {
            MenuSelection++;
        }
        break;
    case SDLK_LEFT:
        
        MenuSelection =0;
        soundManager->loadSound("Musics/SelectionMove.mp3", "SelectionMove"); // 효과음 로드
        soundManager->playSound("SelectionMove", 0);
        break;
    case SDLK_SPACE:
    case SDLK_RETURN:
        
        std::cout << "Selected Menu: " << MenuSelection << std::endl;
        soundManager->loadSound("Musics/Selection.mp3", "Selection"); // 효과음 로드
        soundManager->playSound("Selection", 0);
        Multi::resetInstance();

        Multi* client = Multi::getInstance();
        switch (MenuSelection)
        {

        case 0: // random mode
          //  drawLoading();
            try {
                std::thread randomRoomThreadObj(randomRoomThread, client);

                // 스레드를 detach 또는 join 하여 관리
                randomRoomThreadObj.detach();
            }
            catch (const std::exception& e) {
                
                std::cerr << "Exception caught: " << e.what() << std::endl;
            }
            sceneManager.changeScene(std::make_unique<LoadingScene>(windowManager, sceneManager));
            break; 
        case 1: // make room
            try {
                std::thread createRoomThreadObj(createRoomThread, client);

                // 스레드를 detach 또는 join 하여 관리
                createRoomThreadObj.detach();
            }
            catch (const std::exception& e) {

                std::cerr << "Exception caught: " << e.what() << std::endl;
            }
         
            break;
        case 2: //enter room code 

            sceneManager.changeScene(std::make_unique<joinRoomScene>(windowManager, sceneManager));
                
          break;

        default:
            break;
        }

        break;

        break;
    }
}



void MutiMenuScene::drawSelection() {
    int posX ;
    int posY ; // 선택된 항목에 따라 Y 위치 조정
    Print* pt = print;

    switch (MenuSelection)
    {
    case 0 : 
        posX = 170;
        posY = 388;
        break;
    case 1:
        posX = 772;
        posY = 388;
        break;
    case 2:
        posX = 742;
        posY = 466;
        break;
    default:
        posX = 170;
        posY = 388;
        break;
    }
    // Delete and print the PNG at the calculated position
    pt->deletePNG("Selection2.png");
    pt->printPNG("Selection2.png", posX, posY, 3);
}

void MutiMenuScene::drawInit() {
    Print* pt = print;
    pt->printPNG("BackGround.png", 0, 0, 0);
    pt->printPNG("Group 105.png", 92, 92, 1);
    pt->printPNG("Group 106.png", 742, 92, 1);
    pt->printPNG("Line 22.png", 600, 0, 3);
    pt->printPNG("image 38.png", 819, 381, 2);
    pt->printPNG("image 39.png", 217, 381, 2);
    pt->printPNG("image 40.png", 789, 459, 2);
    pt->printPNG("image 16.png", 967, 587, 2);

    // 초기 선택 상태 설정
    int posX = 170;
    int posY = 388;
    pt->printPNG("Selection2.png", posX, posY, 3);

}



void MutiMenuScene::drawLoading() {
 
    print->printPNG("BackGround.png", 0, 0, 10);
    std::vector<std::string> animPaths1 = {"Loading1.png","Loading2.png","Loading3.png"};
   
   // 현재 인덱스의 이미지를 출력
    print->printAnimationPNG(animPaths1, 917, 572, 11,60);

}

void MutiMenuScene::deleteLoading() {

    print->deleteLayer(10);
    print->deleteLayer(11);

}

void MutiMenuScene::drawCode() {

    print->printPNG("BackGround.png", 0, 0, 10);
    TTF_Font* font = print->loadFont("DungGeunMo.ttf", 60);
    SDL_Color color = { 255, 255, 255 }; // 흰색
    print->printText("Code: "+std::to_string(Multi::getInstance()->roomCode), 420, 307, 11, font, color);
}

void MutiMenuScene::deleteCode() {

    print->deleteLayer(10);
    print->deleteLayer(11);
    //print->setText(11," ");

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

    if (!isCodeScene && Multi::getInstance()->roomCode != 0) {
        isCodeScene = true;
        drawCode();
    }

    if (isCodeScene && Multi::getInstance()->isReady) {
        sceneManager.changeScene(std::make_unique<MultiGameScene>(windowManager, sceneManager));

    }
}

void MutiMenuScene::render() {
    
    drawSelection();
    print->updateAnimations();
    print->render();
}
