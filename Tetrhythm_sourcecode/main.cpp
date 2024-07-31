#include "Print.h"
#include "WindowManager.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include "SceneManager.h"
#include "MainMenu.h"
#include "LoginScene.h"

int main(int argc, char* argv[]) {
    //유저정보 싱글톤
    
    // 프레임 속도: 60
    const int targetFPS = 60;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    
    WindowManager windowManager("Tetrhythm", 1200, 675);
    SceneManager sceneManager;

    //시작 시 호출 원하는 씬을 여기서 부르면 됨. 씬 상속 받은 클래스만 가능
    sceneManager.changeScene(std::make_unique<LoginScene>(windowManager,sceneManager));

    while (true) {
 
    
        sceneManager.update();
        sceneManager.render();
        sceneManager.handleEvents();

        std::this_thread::sleep_for(frameDuration);
    }

    return 0;
}