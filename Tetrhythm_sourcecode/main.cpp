#define WIN32_LEAN_AND_MEAN
#include "MutiMenuScene.h"
#include "Print.h"
#include "WindowManager.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include "SceneManager.h"
#include "MainMenu.h"
#include "Characters.h"
#include "LoginScene.h"
#include "TetrisScene.h"
#include "GameOverScene.h"


int main(int argc, char* argv[]) {
    //유저정보 싱글톤
    
    // 프레임 속도: 60
    const int targetFPS = 60;
    const std::chrono::milliseconds frameDuration(1000 / targetFPS);
    
    WindowManager windowManager("Tetrhythm", 1200, 675);
    SceneManager sceneManager;

    //시작 시 호출 원하는 씬을 여기서 부르면 됨. 씬 상속 받은 클래스만 가능
    //test
    UserInfo::getInstance().setUserID("ZiZonYujin");//MutiMenuScene
    sceneManager.changeScene(std::make_unique<MutiMenuScene>(windowManager,sceneManager));

    while (true) {
 
    
        sceneManager.update();
        sceneManager.render();
        sceneManager.handleEvents();

        std::this_thread::sleep_for(frameDuration);
    }

    return 0;
}



//#include <iostream>
//#include <winsock2.h>
//#include <thread>
//#include <ws2tcpip.h>
//using namespace std;
//
//#define PACKET_SIZE 1024
//#pragma comment(lib, "Ws2_32.lib")
//
//
//void recv_data(SOCKET& s) {
//	char buf[PACKET_SIZE];
//
//	while (1) {
//		ZeroMemory(buf, PACKET_SIZE);
//		recv(s, buf, PACKET_SIZE, 0);
//
//		if (WSAGetLastError()) break; //서버종료 감지
//
//		cout << "\n[Server] >> " << buf << "\ndata >> ";
//	}
//
//	return;
//}
//
//void openSocket(char IP[], int PORT) {
//	WSADATA wsa;
//	if (WSAStartup(MAKEWORD(2, 2), &wsa)) {
//		cout << "WSA error";
//		WSACleanup();
//		return;
//	}
//
//	SOCKET skt;
//	skt = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (skt == INVALID_SOCKET) {
//		cout << "socket error";
//		closesocket(skt);
//		WSACleanup();
//		return;
//	}
//
//	SOCKADDR_IN addr = {};
//	addr.sin_family = AF_INET;
//	addr.sin_port = htons(PORT);
//	addr.sin_addr.s_addr = inet_addr(IP);
//
//
//	while (connect(skt, (SOCKADDR*)&addr, sizeof(addr)));
//
//	char buf[PACKET_SIZE];
//	wchar_t titleWide[PACKET_SIZE];
//
//	recv(skt, buf, PACKET_SIZE, 0);
//	int mynum = atoi(buf);
//	sprintf_s(buf, "[%d] %s::%d", mynum, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
//
//	MultiByteToWideChar(CP_ACP, 0, buf, -1, titleWide, sizeof(titleWide) / sizeof(titleWide[0]));
//	SetConsoleTitle(titleWide);
//
//	thread(recv_data, ref(skt)).detach();
//
//	while (!WSAGetLastError()) {
//		cout << "data >> ";
//		cin >> buf;
//
//		send(skt, buf, strlen(buf), 0); // 데이터전송
//	}
//
//	closesocket(skt);
//	WSACleanup();
//}
//
//int main1(int argc, char* argv[]) {
//	char IP[100];
//	int PORT;
//	cout << "ip>> ";
//	cin >> IP;
//	cout << "port >> ";
//	cin >> PORT;
//
//	openSocket(IP, PORT);
//	return 0;
//}
