//#include "Print.h"
//#include "WindowManager.h"
//#include <vector>
//#include <thread>
//#include <chrono>
//#include <iostream>
//
//int main(int argc, char* argv[]) {
//    WindowManager windowManager("Tetrhythm", 1200, 675);
//    Print* print = new Print(&windowManager);
//
//    // 텍스트 출력 테스트
//    TTF_Font* font = print->loadFont("Galmuri9.ttf", 28); // 폰트출력추가
//    if (!font) return -1;
//
//    SDL_Color color = { 255, 255, 255 }; // 흰색
//    print->printText("Input ID", 200, 100, 1, font, color); // 폰트출력추가
//
//    // PNG 출력 테스트
//    print->printPNG("pngImage.png", 300, 300, 2);
//    print->printPNG("testPic.png", 50, 50, 3);
//
//    // 애니메이션 출력 테스트
//    std::vector<std::string> animPaths1 = { "frame1.png", "frame2.png" };
//    print->printAnimationPNG(animPaths1, 200, 200, 4, 60);
//
//    const int targetFPS = 60;
//    const std::chrono::milliseconds frameDuration(1000 / targetFPS);
//    int i = 0;
//    //텍스트 input
//    
//    TTF_Font* font2 = print->loadFont("Galmuri9.ttf", 28); // 폰트출력추가
//    print->InputText(200, 200, 1, font2, color);
//
//    SDL_StartTextInput();
//
//    while (true) {
//        i++;
//        //폰트 입력 동시에 했을 때는 이렇게.
//        print->updateAnimations();
//        print->handleTextEvents();
//        print->renderWithTextInputAndPNG();
//
//        if (i > 60) {
//            print->setText("BBuuBBuu");
//        }
//
//        std::this_thread::sleep_for(frameDuration);
//    }
//    SDL_StopTextInput();
//    print->unloadFont(font); // 폰트출력추가
//    delete print;
//    return 0;
//}
