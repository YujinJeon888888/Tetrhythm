#include "Print.h"

int main(int argc, char* argv[]) {
    Print print;

    // 여러 이미지를 서로 다른 위치에 출력
    print.printPNG("pngImage.png", 400, 200, 1); // 레이어 1
    print.printPNG("testPic.png", 50, 50, 2); // 레이어 2

    // 애니메이션 출력
    std::vector<std::string> animPaths = { "frame1.png", "frame2.png" };
    print.printAnimationPNG(animPaths, 200, 200, 1, 60); // 60 프레임마다 애니메이션 출력

    while (true) {

        print.run();
    }

    return 0;
}
