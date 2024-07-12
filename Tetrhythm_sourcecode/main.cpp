#include "Print.h"
#include <vector>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
    Print print;
    
    // ���� �̹����� ���� �ٸ� ��ġ�� ���
    print.printPNG("pngImage.png", 300, 300, 1); // ���̾� 1
    print.printPNG("testPic.png", 50, 50, 2); // ���̾� 2

    // �ִϸ��̼� ���
    std::vector<std::string> animPaths1 = { "frame1.png", "frame2.png" };
    print.printAnimationPNG(animPaths1, 200, 200, 3, 60); // 1�ʸ��� �ִϸ��̼� ���
    std::vector<std::string> animPaths2 = { "Anim1.png", "Anim2.png" };
    print.printAnimationPNG(animPaths2, 200, 200, 4, 60); // 1�ʸ��� �ִϸ��̼� ���

    const int targetFPS = 60;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);

    while (true) {
        print.handleEvents();
        print.updateAnimations(); // �ִϸ��̼� ������Ʈ
        print.render(); // ������
        
        // ������ �ӵ� ����
        std::this_thread::sleep_for(frameDuration);
    }

    return 0;
}
