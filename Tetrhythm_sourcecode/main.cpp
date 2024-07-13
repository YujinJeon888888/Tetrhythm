#include "Print.h"
#include "WindowManager.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

int main(int argc, char* argv[]) {
    WindowManager windowManager("Tetrhythm", 1200, 675);

    Print* print = new Print(&windowManager);

    // 여러 이미지를 서로 다른 위치에 출력
    print->printPNG("pngImage.png", 300, 300, 1); // 레이어 1
    print->printPNG("testPic.png", 50, 50, 2); // 레이어 2

    // 애니메이션 출력
    std::vector<std::string> animPaths1 = { "frame1.png", "frame2.png" };
    print->printAnimationPNG(animPaths1, 200, 200, 3, 60); // 1초마다 애니메이션 출력
    std::vector<std::string> animPaths2 = { "Anim1.png", "Anim2.png" };
    print->printAnimationPNG(animPaths2, 200, 200, 4, 60); // 1초마다 애니메이션 출력

    const int targetFPS = 60;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    int i = 0;
    while (true) {

        print->handleEvents();
        print->updateAnimations(); // 애니메이션 업데이트
        print->render(); // 렌더링

        // 프레임 속도 제어
        std::this_thread::sleep_for(frameDuration);

        i++;
        if (i == 240) {
            // 화면을 지우고 print 객체 삭제
            print->clearScreen();
            delete print;
            break;
        }
    }


    while (true) {
        std::this_thread::sleep_for(frameDuration*60);
        std::cout << "화면클리어, print객체파괴" << std:: endl;
    }

    return 0;
}
