#include "Renderer.h"
#include "LayerType.h"


/// <summary>
/// 이미지 출력:
    //Renderer 객체를 생성합니다.
    //addLayer 메서드를 사용하여 이미지를 추가합니다.
    //run 메서드를 호출하여 렌더링 루프를 실행합니다.
//애니메이션 출력 :
//
    //Renderer 객체를 생성합니다.
    //addAnimation 메서드를 사용하여 애니메이션을 추가합니다.
    //run 메서드를 호출하여 렌더링 루프를 실행합니다.
//이미지 레이어 순서 조정 :
//
    //addLayer 메서드를 사용하여 이미지를 추가할 때, z - index와 LayerType을 설정합니다.
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main(int argc, char* argv[]) {
    const int windowWidth = 1600;
    const int windowHeight = 900;

    Renderer renderer(windowWidth, windowHeight);

    // 레이어 추가 (경로와 위치, z-index, 타입 설정)
    renderer.addLayer("testPic.png", 0, 0, 0, UI); // z-index 0, UI 타입
    renderer.addLayer("pngImage.png", 300, 300, -1, BACKGROUND); // z-index -1, BACKGROUND 타입

    // 애니메이션 추가 (이미지 경로 리스트, 위치, z-index, 타입, 프레임 딜레이 설정)
    std::vector<std::string> animationFrames = {
        "frame1.png",
        "frame2.png",
    };
    renderer.addAnimation(animationFrames, 0, 0, 0, GAME, 500); // 500ms (0.5초)마다 프레임 변경

    // 렌더링 실행
    renderer.run();

    return 0;
}
