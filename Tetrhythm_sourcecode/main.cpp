#include "Renderer.h"
#include "LayerType.h"


/// <summary>
/// �̹��� ���:
    //Renderer ��ü�� �����մϴ�.
    //addLayer �޼��带 ����Ͽ� �̹����� �߰��մϴ�.
    //run �޼��带 ȣ���Ͽ� ������ ������ �����մϴ�.
//�ִϸ��̼� ��� :
//
    //Renderer ��ü�� �����մϴ�.
    //addAnimation �޼��带 ����Ͽ� �ִϸ��̼��� �߰��մϴ�.
    //run �޼��带 ȣ���Ͽ� ������ ������ �����մϴ�.
//�̹��� ���̾� ���� ���� :
//
    //addLayer �޼��带 ����Ͽ� �̹����� �߰��� ��, z - index�� LayerType�� �����մϴ�.
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main(int argc, char* argv[]) {
    const int windowWidth = 1600;
    const int windowHeight = 900;

    Renderer renderer(windowWidth, windowHeight);

    // ���̾� �߰� (��ο� ��ġ, z-index, Ÿ�� ����)
    renderer.addLayer("testPic.png", 0, 0, 0, UI); // z-index 0, UI Ÿ��
    renderer.addLayer("pngImage.png", 300, 300, -1, BACKGROUND); // z-index -1, BACKGROUND Ÿ��

    // �ִϸ��̼� �߰� (�̹��� ��� ����Ʈ, ��ġ, z-index, Ÿ��, ������ ������ ����)
    std::vector<std::string> animationFrames = {
        "frame1.png",
        "frame2.png",
    };
    renderer.addAnimation(animationFrames, 0, 0, 0, GAME, 500); // 500ms (0.5��)���� ������ ����

    // ������ ����
    renderer.run();

    return 0;
}
