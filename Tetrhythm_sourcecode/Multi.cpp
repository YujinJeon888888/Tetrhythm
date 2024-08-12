#include "Multi.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS

Multi* Multi::instance = nullptr;

Multi::Multi() {

}


Multi* Multi::getInstance() {
    if (instance == nullptr) {
        instance = new Multi();
    }
    return instance;
}

int Multi::getRandomRoom() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");//"52.14.83.66"
    serverAddr.sin_port = htons(8080);

    connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    // 서버로부터 방 번호 수신
    char buffer[1024];
    int len = recv(clientSocket, buffer, 1024, 0);
    buffer[len] = '\0';
    int roomNumber = atoi(buffer);
    std::cout << "Room Number From Server: " << roomNumber << std::endl;



    while (true) {
        len = recv(clientSocket, buffer, 1024, 0);
        if (len > 0) {

            if (memcmp(buffer, "full", strlen("full")) == 0) {
                std::cout << "The game is ready\n";
                break;
            }
            else {
                std::cout << buffer << std::endl;
                return -1;
            }
        }
    }


    // Create threads for receiving and sending messages
    std::thread receiveThread(&Multi::receiveMessages,this);
    std::thread sendThread(&Multi::sendMessages,this);

    // Wait for both threads to finish
    receiveThread.join();
    sendThread.join();


    closesocket(clientSocket);
    WSACleanup();

    return roomNumber;
}

void Multi::receiveMessages() {
    char buffer[1024];
    int len;


    while (true) {
        len = recv(clientSocket, buffer, 1024, 0);
        if (len > 0) {
            buffer[len] = '\0';
            std::cout << "\nopponent: " << buffer << std::endl;
        }
       else {
            std::cout << "Connection closed by the opponent." << std::endl;
            break;
        }
    }
}

void Multi::sendMessages() {
    std::string message;

    while (true) {
        std::cout << "chat: ";
        std::getline(std::cin, message);
        if (send(clientSocket, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send message." << std::endl;
           
        }
    }
}

void Multi::closeConnection() {
    closesocket(clientSocket);
    WSACleanup();
}

Multi::~Multi() {
    closeConnection();
}
