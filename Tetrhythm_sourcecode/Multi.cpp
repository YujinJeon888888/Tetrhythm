#include "Multi.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS

Multi* Multi::instance = nullptr;

Multi::Multi() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("52.14.83.66");//"52.14.83.66"
    serverAddr.sin_port = htons(8080);

    connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

}


Multi* Multi::getInstance() {
    if (instance == nullptr) {
        instance = new Multi();
    }
    return instance;
}

int Multi::getRandomRoom() {

    std::string msg = "random_room";
    if (send(clientSocket, msg.c_str(), msg.size(), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message." << std::endl;

    }
    
    // 서버로부터 방 번호 수신
    char buffer[1024];
    int len = recv(clientSocket, buffer, 1024, 0);
    int roomNumber;


    if (len < 0) { std::cout << "Server Error\n";  return -1; }

      buffer[len] = '\0';
      roomNumber = atoi(buffer);
      std::cout << "Room Number From Server: " << roomNumber << std::endl;
   
      connetOpponent();

 

    return roomNumber;
}

int Multi::createRoom() {

    std::string msg = "create_room";
    if (send(clientSocket, msg.c_str(), msg.size(), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message." << std::endl;
    }

    // 서버로부터 방 번호 수신
    char buffer[1024];
    int len = recv(clientSocket, buffer, 1024, 0);
    int roomNumber;

    if (len < 0) { std::cout << "Server Error\n";  return -1; }

    buffer[len] = '\0';
    roomNumber = atoi(buffer);
    std::cout << "Room Password From Server: " << roomNumber << std::endl;

    connetOpponent();

    return roomNumber;
}

int Multi::joinRoom(std::string password) {

    std::string msg = "join_room";
    if (send(clientSocket, msg.c_str(), msg.size(), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message." << std::endl;

        return -1;
    }

    if (send(clientSocket, password.c_str(), password.size(), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message." << std::endl;
        return -1;
    }

    std::cout << "You enter room " << password << std::endl;

    connetOpponent();

    return stoi(password);
}




void Multi::connetOpponent() {

    char buffer[1024];
    int len = recv(clientSocket, buffer, 1024, 0);

    if (len > 0) {

        if (memcmp(buffer, "ready", strlen("ready")) == 0) {
            std::cout << "The game is ready\n";
            //한번 더 체크 해야함.
            isReady = true;

        }
        else {
            std::cout << buffer << std::endl;
            return ;
        }
    }

    std::cout << "The game is ready??\n";

    // Create threads for receiving and sending messages
    std::thread receiveThread(&Multi::receiveMessages, this);
    std::thread sendThread(&Multi::sendMessages, this);

    // Wait for both threads to finish
    receiveThread.join();
    sendThread.join();


    closesocket(clientSocket);
    WSACleanup();
}

void Multi::receiveMessages() {
    char buffer[1024];
    int len;

    while (true) {
        len = recv(clientSocket, buffer, 1024, 0);
        if (len > 0) {
            buffer[len] = '\0';
 
            // 상대방의 메시지를 출력
            std::cout << "\r" << std::string(100, ' ') << "\r"; // 현재 줄을 지우기 위한 코드
            std::cout << "opponent: " << buffer << std::endl;

            // 입력 중이던 텍스트를 다시 출력
             std::cout << "chat: " ;
             std::flush(std::cout);
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

    send(clientSocket, "exit", strlen("exit"), 0);
    closeConnection();
}
