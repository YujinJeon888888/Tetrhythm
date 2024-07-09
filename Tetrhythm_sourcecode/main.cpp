#include "Renderer.h"

int main(int argc, char* argv[]) {
    // 창 크기 설정
    const int windowWidth = 800;
    const int windowHeight = 600;

    // Renderer 객체 생성
    Renderer renderer(windowWidth, windowHeight);

    // 레이어 추가 (이미지 경로는 본인의 파일 경로로 수정)
    renderer.addLayer("테스트그림.png");
    // 렌더링 실행
    renderer.run();

    return 0;
}
