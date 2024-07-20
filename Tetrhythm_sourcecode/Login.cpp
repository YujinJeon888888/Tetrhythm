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
//
//    // SDL 마우스 커서를 보이도록 설정
//    SDL_ShowCursor(SDL_ENABLE);
//    SDL_SetRelativeMouseMode(SDL_FALSE);
//
//    // 마우스 커서 초기 위치 설정
//    int cursorX = 30;
//    int cursorY = 30;
//    bool cursorMoved = false;
//
//    while (true) {
//        // 이벤트 처리
//        print->handleEvents();
//
//        // 애니메이션 업데이트 및 렌더링
//        print->updateAnimations();
//        print->render();
//
//        // 마우스 커서 위치 설정
//        if (!cursorMoved) {
//            SDL_WarpMouseInWindow(windowManager.getWindow(), cursorX, cursorY);
//            std::cout << "마우스 커서가 (" << cursorX << ", " << cursorY << ")로 이동되었습니다." << std::endl;
//            cursorMoved = true;
//        }
//
//        // 프레임 속도 제어
//        std::this_thread::sleep_for(frameDuration);
//    }
//
//    return 0;
//}
