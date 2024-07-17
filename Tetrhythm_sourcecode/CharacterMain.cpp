#include "WindowManager.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include "Characters.h"
#include "Print.h"


int main(int argc, char* argv[]) {

    //프레임 속도: 60
    const int targetFPS = 60;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    WindowManager windowManager("Tetrhythm", 1200, 675);


    Print* print = new Print(&windowManager); 
    Characters ch;
    ch.drawInit(print);

    while (true) {

 
        print->handleEvents([&](SDL_Event& event) {

            if (event.type == SDL_KEYDOWN) {
                ch.handleArrowKey(event.key.keysym.sym);
            }
            }
        );
        ch.drawSelection(print);

        print->updateAnimations();
        print->render();
 
 
        std::this_thread::sleep_for(frameDuration);
    }


    return 0;
}
