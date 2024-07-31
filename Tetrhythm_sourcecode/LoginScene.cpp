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
    //[this] : 현재 객체의 포인터를 캡처합니다.이를 통해 람다 함수 내부에서 클래스의 멤버 함수와 멤버 변수에 접근할 수 있습니다.
    //    () : 람다 함수가 인자를 받지 않음을 의미합니다.
    //{ this->whenEnter(); } : 람다 함수가 호출될 때 this->whenEnter()를 실행합니다.이는 현재 객체의 whenEnter 멤버 함수를 호출하는 것을 의미합니다.
    print->handleTextEvents([this]() { this->whenEnter(); }, [&](SDL_Event& event) {
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

void LoginScene::whenEnter()
{
    const std::string str = print->getTextInput().c_str();
    MySQL mysql;
    if (str.size() > 12 && str.find(" ") != std::string::npos) {
        print->printPNG("IDInputExepStmtBox.png", 308, 185, print->getLayeredTextures().back().layer + 1);
        print->printPNG("IDInputExepStmtWords.png", 332, 258, print->getLayeredTextures().back().layer + 1);
    }
    else {
        if (str.find(" ") != std::string::npos || mysql.containsInvalidCharacters(str)) {
            //공백 포함 하거나 특수문자 포함 시                        
            print->printPNG("IDInputExepStmtBox.png", 308, 185, print->getLayeredTextures().back().layer + 1);
            print->printPNG("IDInputExepStmSpecWord.png", 366, 256, print->getLayeredTextures().back().layer + 1);
        }
        if (str.size() > 12) {
            print->printPNG("IDInputExepStmtBox.png", 308, 185, print->getLayeredTextures().back().layer + 1);
            print->printPNG("IDInputExepStmtWords.png", 332, 258, print->getLayeredTextures().back().layer + 1);
        }
    }

    if (mysql.isDuflicatedUser(str)) {//중복이면
        print->printPNG("IDInputExepStmtBox.png", 308, 185, print->getLayeredTextures().back().layer + 1);
        print->printPNG("IDInputExepStmtDupli.png", 379, 254, print->getLayeredTextures().back().layer + 1);
    }
    if (!mysql.isDuflicatedUser(str) && str.size() <= 12 && str.find(" ") == std::string::npos && !mysql.containsInvalidCharacters(str)) {
        //중복 아니고 공백 미포함이고 12자 이내이면 아이디 생성
        mysql.insertAndShowUsers(str);
        //유저 정보 싱글톤에 저장
        UserInfo::getInstance().setUserID(str);
        //씬 이동
        sceneManager.changeScene(std::make_unique<MainMenu>(windowManager, sceneManager));
    }

}


void LoginScene::update()
{

}

void LoginScene::render()
{
    print->renderWithTextInputAndPNG();
}

