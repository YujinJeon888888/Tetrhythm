#include "Print.h"
#include "WindowManager.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include "SceneManager.h"
#include "MainMenu.h"


int main(int argc, char* argv[]) {
    // 프레임 속도: 60
    const int targetFPS = 60;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    
    WindowManager windowManager("Tetrhythm", 1200, 675);
    SceneManager sceneManager;

    sceneManager.changeScene(std::make_unique<MainMenu>(windowManager,sceneManager));

    while (true) {
 
    
        sceneManager.update();
        sceneManager.render();
        sceneManager.handleEvents();

        std::this_thread::sleep_for(frameDuration);
    }

    return 0;
}