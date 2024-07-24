
// SceneManager.cpp
#include "SceneManager.h"

void SceneManager::changeScene(std::unique_ptr<Scene> newScene) {
 
    scenes.push(std::move(newScene));
}

void SceneManager::pushScene(std::unique_ptr<Scene> newScene) {
    scenes.push(std::move(newScene));
}

void SceneManager::popScene() {
    if (!scenes.empty()) {
        scenes.pop();
    }
}

void SceneManager::goBack() {
    if (scenes.size() > 1) { //씬이 두개 이상일 때만
        popScene();  // 현재 씬을 제거하고, 이전 씬을 다시 보여줍니다.

    }
}


void SceneManager::handleEvents() {
    if (!scenes.empty()) {
        scenes.top()->handleEvents();
    }
}

void SceneManager::update() {
    if (!scenes.empty()) {
        scenes.top()->update();
    }
}

void SceneManager::render() {
    if (!scenes.empty()) {
        scenes.top()->render();
    }
}
