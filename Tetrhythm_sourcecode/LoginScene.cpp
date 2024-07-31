#include "LoginScene.h"
#include "MySQL.h"

LoginScene::LoginScene(WindowManager& wm, SceneManager& manager) 
    //객체 생성 및 연결
    : windowManager(wm), sceneManager(manager), print(new Print(&wm))
{
    drawInit();
}

void LoginScene::drawInit()
{
    //객체 연결
    Print* pt = print;
    //프린트 세팅
    print->printPNG("IDInputBox.png", 375, 337.5, 1);
    print->printPNG("InputID.png", 271.5, 155.25, 2);
    print->printPNG("ToCompleteSpace.png", 908.25, 582, 3);
    //input text 세팅
    TTF_Font* font = print->loadFont("DungGeunMo.ttf", 30);
    SDL_Color color = { 255, 255, 255 }; // 흰색
    print->InputText(390, 360, 4, font, color);
    SDL_StartTextInput();

}

void LoginScene::handleEvents()
{
    print->handleTextEvents([&](SDL_Event& event) {
            handleClosePopup(event.key.keysym.sym);
        });
}

void LoginScene::handleClosePopup(SDL_Keycode key) {
    if (key == SDLK_ESCAPE && print->getLayeredTextures().back().path == "IDInputExepStmtDupli.png") {
        print->deletePNG("IDInputExepStmtBox.png");
        print->deletePNG("IDInputExepStmtDupli.png");
    }
    //글자 수 초과 팝업창 닫기
    if (key == SDLK_ESCAPE && print->getLayeredTextures().back().path == "IDInputExepStmtWords.png") {
        print->deletePNG("IDInputExepStmtBox.png");
        print->deletePNG("IDInputExepStmtWords.png");
    }
    //공백 경고 팝업창 닫기
    if (key == SDLK_ESCAPE && print->getLayeredTextures().back().path == "IDInputExepStmSpecWord.png") {
        print->deletePNG("IDInputExepStmtBox.png");
        print->deletePNG("IDInputExepStmSpecWord.png");
    }

}


void LoginScene::update()
{

}

void LoginScene::render()
{
    print->renderWithTextInputAndPNG();
}

