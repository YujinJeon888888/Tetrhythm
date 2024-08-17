#include "createRoomScene.h"


createRoomScene::createRoomScene(WindowManager& wm, SceneManager& manager)
	: windowManager(wm), sceneManager(manager), print(new Print(&wm))
{

	if (Multi::getInstance()->roomCode == 0)
		sceneManager.goBack();

	drawInit();
	
}

void createRoomScene::handleArrowKey(SDL_Keycode key) {
	switch (key) {

	case SDLK_ESCAPE:

		sceneManager.goBack();

	}

}
void createRoomScene::drawInit() {

	print->printPNG("BackGround.png", 0, 0, 10);
	TTF_Font* font = print->loadFont("DungGeunMo.ttf", 60);
	SDL_Color color = { 255, 255, 255 }; // 흰색
	print->printText("Code: " + std::to_string(Multi::getInstance()->roomCode), 420, 307, 11, font, color);

}

void createRoomScene::handleEvents() {
	print->handleEvents([&](SDL_Event& event) {
		if (event.type == SDL_KEYDOWN) {
			handleArrowKey(event.key.keysym.sym);
		}
		});
}

void createRoomScene::update()
{
}

void createRoomScene::render()
{
	print->updateAnimations();
	print->render();
}
