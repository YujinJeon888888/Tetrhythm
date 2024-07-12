#include <vector>
#include "Characters.h"
#include "Print.h"

int main(int argc, char* argv[]) {
    Print print;
    
    // ���� �̹����� ���� �ٸ� ��ġ�� ���
    //print.printPNG("pngImage.png", 300, 300, 1); // ���̾� 1, ���̾ ���� ���� z ���� ����.
    //print.printPNG("testPic.png", 50, 50, 2); // ���̾� 2

    // �ִϸ��̼� ���
    std::vector<std::string> animPaths1 = { "frame1.png", "frame2.png" };
    //print.printAnimationPNG(animPaths1, 200, 200, 3, 240); // 240 �����Ӹ��� �ִϸ��̼� ���
    
    Characters ch;

    ch.draw(&print);
   // ch.draw(print.getRenderer());
   while (true) {
   
      //print.handleEvents();
      //print.updateAnimations(); // �ִϸ��̼� ������Ʈ
      print.render(); // ������
    }

    return 0;
}
