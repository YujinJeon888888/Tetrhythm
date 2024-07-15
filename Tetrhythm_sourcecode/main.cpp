#include "Print.h"
#include "WindowManager.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

int main(int argc, char* argv[]) {
    //������ â ����
    WindowManager windowManager("Tetrhythm", 1200, 675);
    //������: 60
    const int targetFPS = 60;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);


    while (true) {
        

        // ������ �ӵ� ����
        std::this_thread::sleep_for(frameDuration);
    }


    return 0;
}
