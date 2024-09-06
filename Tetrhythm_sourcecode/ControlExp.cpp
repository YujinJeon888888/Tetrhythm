#include "ControlExp.h"
#include "MainMenu.h"

ControlExp::ControlExp(WindowManager& wm, SceneManager& manager)
	: windowManager(wm), sceneManager(manager), print(new Print(&wm))
{
	drawInit();
}

void ControlExp::drawInit()
{
	print->printPNG("Resources/BackGround.png", 0, 0, 0);
	print->printPNG("Resources/arrow 1.png", 213, 136, 1);
	print->printPNG("Resources/spc 1.png", 257, 267, 2);
	print->printPNG("Resources/w 1.png", 251, 398, 3);
	print->printPNG("Resources/z 1.png", 321, 398, 4);
	print->printPNG("Resources/d 1.png", 251, 518, 5);
	print->printPNG("Resources/c 1.png", 321, 518, 6);
	print->printPNG("Resources/moveKey.png", 500, 146, 7);
	print->printPNG("Resources/hardDrop.png", 500, 277, 8);
	print->printPNG("Resources/BanClockRotate.png", 500, 408, 9);
	print->printPNG("Resources/clockRotate.png", 500, 528, 10);

	// 애니메이션 출력
	std::vector<std::string> animPaths1 = { "Resources/CholangSands1.png","Resources/CholangSands2.png","Resources/CholangSands3.png","Resources/CholangSands4.png","Resources/CholangSands5.png","Resources/CholangSands6.png" };
	print->printAnimationPNG(animPaths1, 780, 300, 11, 7); // 1초마다 애니메이션 출력

	// 텍스트 애니메이션 초기화
	font = print->loadFont("Resources/DungGeunMo.ttf", 85);
	print->textAnimation("Controls", 820, 71, font, colors, 15, 12);
}

void ControlExp::handleEvents()
{
	print->handleEvents();
}

void ControlExp::update()
{
	print->updateAnimations();
	print->updateTextAnimation();
	print->handleEvents([&](SDL_Event& event) {
		if (event.type == SDL_KEYDOWN) {
			handleArrowKey(event.key.keysym.sym);
		}
		});

}
void ControlExp::handleArrowKey(SDL_Keycode key) {

	switch (key) {
	case SDLK_ESCAPE:
		sceneManager.changeScene(std::make_unique<MainMenu>(windowManager, sceneManager));
		break;
	}
}


void ControlExp::render()
{
	print->render();

}
