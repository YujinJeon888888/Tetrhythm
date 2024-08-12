#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

void receiveMessages(SOCKET clientSocket) {
    char buffer[1024];
    int len;

    while (true) {
        len = recv(clientSocket, buffer, 1024, 0);
        if (len > 0) {
            buffer[len] = '\0';
            std::cout << "\nopponent: " << buffer << std::endl;
        }
        else {
            // 상대방이 연결을 끊었을 경우 처리
            std::cout << "Connection closed by the opponent." << std::endl;
            break;
        }
    }
}


void sendMessages(SOCKET clientSocket) {
    std::string message;

    while (true) {
        std::cout << "chat: ";
        std::getline(std::cin, message);
        if (send(clientSocket, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
            // 전송 오류 처리
            std::cerr << "Failed to send message." << std::endl;
            break;
        }
    }
}


int getRandomRoom() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("52.14.83.66");
    serverAddr.sin_port = htons(8080);

    connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    // 서버로부터 방 번호 수신
    char buffer[1024];
    int len = recv(clientSocket, buffer, 1024, 0);
    buffer[len] = '\0';
    int roomNumber = atoi(buffer);
    std::cout << "Room Number From Server: " << roomNumber << std::endl;

    // 수신 및 전송 스레드 생성
    std::thread receiveThread(receiveMessages, clientSocket);
    std::thread sendThread(sendMessages, clientSocket);

    // 두 스레드가 종료될 때까지 대기
    receiveThread.join();
    sendThread.join();


    closesocket(clientSocket);
    WSACleanup();
    return 0;
}


