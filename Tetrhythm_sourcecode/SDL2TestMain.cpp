#include "Print.h"
#include "WindowManager.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

int main_�����ϰ�������̸���main���ιٲټ���(int argc, char* argv[]) {
    WindowManager windowManager("Tetrhythm", 1200, 675);
    ////////////////////////////

    Print* print = new Print(&windowManager);

    // ���� �̹����� ���� �ٸ� ��ġ�� ���
    print->printPNG("pngImage.png", 300, 300, 1); // ���̾� 1

    // �ִϸ��̼� ���
    std::vector<std::string> animPaths1 = { "Anim1.png", "Anim2.png" };
    print->printAnimationPNG(animPaths1, 200, 200, 2, 60); // 1�ʸ��� �ִϸ��̼� ���
    /////////////////////////////
    Print* print2 = new Print(&windowManager);
    print2->printPNG("testPic.png", 50, 50, 1);
    std::vector<std::string> animPaths2= { "frame1.png", "frame2.png" };
    print2->printAnimationPNG(animPaths2, 200, 200, 2, 60); // 1�ʸ��� �ִϸ��̼� ���

    ///////////////////////////////

    const int targetFPS = 60;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    
    
    while (true) {
        //����ȯ����: 4�ʰ������� ���� ��ȯ
        static int i = 0;
        i++;
        if (i >= 240&&i<=480) {
            if (i == 480) {
                i = 0;
            }
            print->handleEvents();
            print->updateAnimations(); // �ִϸ��̼� ������Ʈ
            print->render(); // ������
        }
        else {
            print2->handleEvents();
            print2->updateAnimations();
            print2->render();
        }


        // ������ �ӵ� ����
        std::this_thread::sleep_for(frameDuration);
    }


    return 0;
}
