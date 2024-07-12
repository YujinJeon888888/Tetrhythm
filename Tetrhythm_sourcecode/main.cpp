#include "Print.h"
#include <vector>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
    Print print;
    
    // 여러 이미지를 서로 다른 위치에 출력
    print.printPNG("pngImage.png", 300, 300, 1); // 레이어 1
    print.printPNG("testPic.png", 50, 50, 2); // 레이어 2

    // 애니메이션 출력
    std::vector<std::string> animPaths1 = { "frame1.png", "frame2.png" };
    print.printAnimationPNG(animPaths1, 200, 200, 3, 60); // 1초마다 애니메이션 출력
    std::vector<std::string> animPaths2 = { "Anim1.png", "Anim2.png" };
    print.printAnimationPNG(animPaths2, 200, 200, 4, 60); // 1초마다 애니메이션 출력

    const int targetFPS = 60;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);

    while (true) {
        print.handleEvents();
        print.updateAnimations(); // 애니메이션 업데이트
        print.render(); // 렌더링
        
        // 프레임 속도 제어
        std::this_thread::sleep_for(frameDuration);
    }

    return 0;
}
