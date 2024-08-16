#include "SceneManager.h"
#include "MySQL.h"
#include <string>
#include <sstream>

class scoreTableScene :public Scene {
public:
	scoreTableScene(WindowManager& wm, SceneManager& manager); // 클래스 이름에 맞게 생성자 이름 변경 해줘야함.
	void drawInit() override;
	void handleEvents() override;
	void update() override;
	void render() override;
private:
	Print* print;
	WindowManager& windowManager;
	SceneManager& sceneManager;
	void handleArrowKey(SDL_Keycode key);
	// 함수 선언부
	std::vector<std::string> splitString(const std::string& str, char delimiter) {
		std::vector<std::string> tokens;
		std::stringstream ss(str);
		std::string token;

		while (std::getline(ss, token, delimiter)) {
			tokens.push_back(token);
		}

		return tokens;
	}
};