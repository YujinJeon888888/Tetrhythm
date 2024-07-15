#include "Print.h"
#include "WindowManager.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

int main1(int argc, char* argv[]) {
    //占쏙옙占쏙옙占쏙옙 창 占쏙옙占쏙옙
    WindowManager windowManager("Tetrhythm", 1200, 675);
    //프레임 속도: 60
    const int targetFPS = 60;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);


    while (true) {
        

        // 占쏙옙占쏙옙占쏙옙 占쌈듸옙 占쏙옙占쏙옙
        std::this_thread::sleep_for(frameDuration);
    }


    return 0;
}
