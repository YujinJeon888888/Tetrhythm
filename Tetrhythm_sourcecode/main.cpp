#include "Print.h"
#include "WindowManager.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

int main1(int argc, char* argv[]) {
    //?좎룞?쇿뜝?숈삕?좎룞??李??좎룞?쇿뜝?숈삕
    WindowManager windowManager("Tetrhythm", 1200, 675);
    //?꾨젅???띾룄: 60
    const int targetFPS = 60;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);


    while (true) {
        

        // ?좎룞?쇿뜝?숈삕?좎룞???좎뙂?몄삕 ?좎룞?쇿뜝?숈삕
        std::this_thread::sleep_for(frameDuration);
    }


    return 0;
}
