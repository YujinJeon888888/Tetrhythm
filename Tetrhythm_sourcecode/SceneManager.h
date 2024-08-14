// SceneManager.h

#pragma once
#include <memory>
#include <stack>
#include "Scene.h"

class SceneManager {
public:
    void changeScene(std::unique_ptr<Scene> newScene);
    void pushScene(std::unique_ptr<Scene> newScene);
    void popScene();
    void goBack();
    void handleEvents();
    void update();
    void render();

    void quit();
    bool isRunning() const;

private:
    std::stack<std::unique_ptr<Scene>> scenes;
    bool running = true;
};
