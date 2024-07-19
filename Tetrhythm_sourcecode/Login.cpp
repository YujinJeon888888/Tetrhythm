#include "Print.h"
#include "WindowManager.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>


int main(int argc, char* argv[]) {
    WindowManager windowManager("Tetrhythm", 1200, 675);
    const int targetFPS = 60;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    Print* print = new Print(&windowManager);
    print->printPNG("IDInputBox.png", 375, 337.5, 1); 
    print->printPNG("InputID.png", 271.5, 155.25, 2);
    print->printPNG("ToCompleteSpace.png", 908.25, 582, 3); 

    while (true) {

        print->handleEvents();
        print->updateAnimations();
        print->render();

        std::this_thread::sleep_for(frameDuration);
    }


    return 0;
}
