#include "Renderer.h"

int main(int argc, char* argv[]) {
    // â ũ�� ����
    const int windowWidth = 800;
    const int windowHeight = 600;

    // Renderer ��ü ����
    Renderer renderer(windowWidth, windowHeight);

    // ���̾� �߰� (�̹��� ��δ� ������ ���� ��η� ����)
    renderer.addLayer("�׽�Ʈ�׸�.png");
    // ������ ����
    renderer.run();

    return 0;
}
