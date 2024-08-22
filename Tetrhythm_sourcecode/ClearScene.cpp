#include "ClearScene.h"

ClearScene::ClearScene(WindowManager& wm, SceneManager& manager, int line, int tetris)
	: windowManager(wm), sceneManager(manager), print(new Print(&wm)), line(line),tetris(tetris)
{
	drawInit();
}

void ClearScene::drawInit()
{
	print->printPNG("BackGround.png", 0, 0, 0);
	print->printPNG("clearImage.png", 259.5, 157.5, 1);
	print->printPNG("clearText.png", 259.5, 26.25, 2);
    //text 세팅
    TTF_Font* font = print->loadFont("DungGeunMo.ttf", 30);
    SDL_Color color = { 255, 255, 255 }; // 흰색
    print->printText("Line: ", 739.5, 237, 3, font, color);
    print->printText("Tetris: ", 739.5, 267, 4, font, color);
    print->printText("Score: ", 739.5, 297, 5, font, color);
    //점수 text
    print->printText("      " + std::to_string(line), 739.5, 237, 6, font, color);
    print->printText("        " + std::to_string(tetris), 739.5, 267, 7, font, color);
    print->printText("       " + std::to_string(UserInfo::getInstance().getScore()), 739.5, 297, 8, font, color);
}

void ClearScene::handleEvents()
{
    print->handleEvents([&](SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            handleArrowKey(event.key.keysym.sym);
        }
        });
}

void ClearScene::update()
{
}

void ClearScene::handleArrowKey(SDL_Keycode key) {
    switch (key) {
    case SDLK_ESCAPE: // 뒤로 가기 기능 추가
        sceneManager.changeScene(std::make_unique<MainMenu>(windowManager, sceneManager));
        break;
    }
}


void ClearScene::render()
{
    print->render();
}