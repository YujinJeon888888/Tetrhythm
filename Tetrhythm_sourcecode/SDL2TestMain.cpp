#include "Print.h"
#include "WindowManager.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

int main_실행하고싶을시이름을main으로바꾸고원래main을주석처리하세요(int argc, char* argv[]) {
    WindowManager windowManager("Tetrhythm", 1200, 675);
    ////////////////////////////

    Print* print1 = new Print(&windowManager);

    // 여러 이미지를 서로 다른 위치에 출력
    print1->printPNG("pngImage.png", 300, 300, 1); // 레이어 1

    // 애니메이션 출력
    std::vector<std::string> animPaths1 = { "Anim1.png", "Anim2.png" };
    print1->printAnimationPNG(animPaths1, 200, 200, 2, 60); // 1초마다 애니메이션 출력
    /////////////////////////////
    Print* print2 = new Print(&windowManager);
    print2->printPNG("testPic.png", 50, 50, 1);
    std::vector<std::string> animPaths2= { "frame1.png", "frame2.png" };
    print2->printAnimationPNG(animPaths2, 200, 200, 2, 60); // 1초마다 애니메이션 출력

    ///////////////////////////////

    const int targetFPS = 60;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    
    
    while (true) {
        //씬전환예시: 4초간격으로 씬이 전환
        static int i = 0;
        i++;
        if (i >= 240&&i<=480) {
            if (i == 480) {
                i = 0;
            }
            print1->moveImage("pngImage.png",(print1->getImagePosition("pngImage.png").x + 1), (print1->getImagePosition("pngImage.png").y + 1));
            print1->handleEvents();
            print1->updateAnimations(); // 애니메이션 업데이트
            print1->render(); // 렌더링
            
        }
        else {
            print2->handleEvents();
            print2->updateAnimations();
            print2->render();
        }


        // 프레임 속도 제어
        std::this_thread::sleep_for(frameDuration);
    }


    return 0;
}
