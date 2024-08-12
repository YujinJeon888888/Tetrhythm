#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
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

    // 메시지 송수신 처리
    std::string message;
    while (true) {

        std::cout << "chat :  ";
        std::getline(std::cin, message);
        send(clientSocket, message.c_str(), message.size(), 0);

        len = recv(clientSocket, buffer, 1024, 0);
        buffer[len] = '\0';
        std::cout << "oppenent: " << buffer << std::endl;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
