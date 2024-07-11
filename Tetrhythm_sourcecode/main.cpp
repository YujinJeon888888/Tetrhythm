#include "Print.h"
#include <vector>

int main(int argc, char* argv[]) {
    Print print;
    
    // 여러 이미지를 서로 다른 위치에 출력
    print.printPNG("pngImage.png", 300, 300, 1); // 레이어 1
    print.printPNG("testPic.png", 50, 50, 2); // 레이어 2

    // 애니메이션 출력
    std::vector<std::string> animPaths1 = { "frame1.png", "frame2.png" };
    print.printAnimationPNG(animPaths1, 200, 200, 3, 240); // 240 프레임마다 애니메이션 출력
    
    while (true) {
        print.handleEvents();
        print.updateAnimations(); // 애니메이션 업데이트
        print.render(); // 렌더링
    }

    return 0;
}
