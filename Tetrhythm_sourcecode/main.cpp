#include "Print.h"

int main(int argc, char* argv[]) {
    Print print;

    // ���� �̹����� ���� �ٸ� ��ġ�� ���
    print.printPNG("pngImage.png", 400, 200, 1); // ���̾� 1
    print.printPNG("testPic.png", 50, 50, 2); // ���̾� 2

    // �ִϸ��̼� ���
    std::vector<std::string> animPaths = { "frame1.png", "frame2.png" };
    print.printAnimationPNG(animPaths, 200, 200, 1, 60); // 60 �����Ӹ��� �ִϸ��̼� ���

    while (true) {

        print.run();
    }

    return 0;
}
