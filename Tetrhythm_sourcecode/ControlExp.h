#pragma once
#include "SceneManager.h"
class ControlExp : public Scene {
public:
	ControlExp(WindowManager& wm, SceneManager& manager); // 클래스 이름에 맞게 생성자 이름 변경 해줘야함.
	void drawInit() override;
	void handleEvents() override;
	void update() override;
	void render() override;
private:
	Print* print;
	WindowManager& windowManager;
	SceneManager& sceneManager;
	std::vector<SDL_Color> colors= {
	{255, 0, 0},      // 빨강
	{255, 165, 0},    // 주황
	{255, 255, 0},    // 노랑
	{0, 255, 0},      // 초록
	{0, 0, 255},      // 파랑
	{75, 0, 130},     // 남색 (인디고)
	{238, 130, 238}   // 보라 (바이올렛)
	};
	TTF_Font* font;
	void handleArrowKey(SDL_Keycode key);

};