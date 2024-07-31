//#include "Print.h"
//#include "WindowManager.h"
//#include <vector>
//#include <thread>
//#include <chrono>
//#include <iostream>
//
//int main(int argc, char* argv[]) {
//    WindowManager windowManager("Tetrhythm", 1200, 675);
//    const int targetFPS = 60;
//    const std::chrono::milliseconds frameDuration(1000 / targetFPS);
//    Print* print = new Print(&windowManager);
//
//    print->printPNG("IDInputBox.png", 375, 337.5, 1);
//    print->printPNG("InputID.png", 271.5, 155.25, 2);
//    print->printPNG("ToCompleteSpace.png", 908.25, 582, 3);
//    //input text
//    TTF_Font* font= print->loadFont("DungGeunMo.ttf", 30);
//    SDL_Color color = { 255, 255, 255 }; // 흰색
//    print->InputText(390, 360, 4 ,font, color);
//    SDL_StartTextInput();
//    while (true) {
//        print->updateAnimations();
//        print->handleTextEvents();
//        print->renderWithTextInputAndPNG();
//
//        // 애니메이션 업데이트 및 렌더링
//        print->updateAnimations();
//
//        // 프레임 속도 제어
//        std::this_thread::sleep_for(frameDuration);
//    }
//
//    return 0;
//}
