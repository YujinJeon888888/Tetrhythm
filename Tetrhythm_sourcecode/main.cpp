#include "Print.h"
#include "WindowManager.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

int main(int argc, char* argv[]) {
    //윈도우 창 생성
    WindowManager windowManager("Tetrhythm", 1200, 675);
    //프레임: 60
    const int targetFPS = 60;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);


    while (true) {
        

        // 프레임 속도 제어
        std::this_thread::sleep_for(frameDuration);
    }


    return 0;
}
