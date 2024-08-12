#include "NetworkManager.h"

// Initialize static member
NetworkManager* NetworkManager::instance = nullptr;

NetworkManager::NetworkManager() {
   
}

void NetworkManager::setupConnection() {

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("52.14.83.66");
    serverAddr.sin_port = htons(8080);

    connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
}

int NetworkManager::receiveRoomNumber() {
    char buffer[1024];
    int len = recv(clientSocket, buffer, 1024, 0);
    buffer[len] = '\0';
    return atoi(buffer);
}

NetworkManager* NetworkManager::getInstance() {
    if (instance == nullptr) {
        instance = new NetworkManager();
    }
    return instance;
}

int NetworkManager::getRandomRoom() {
    setupConnection();
    int roomNumber = receiveRoomNumber();
    std::cout << "Room Number From Server: " << roomNumber << std::endl;

    // Create threads for receiving and sending messages
    std::thread receiveThread(&NetworkManager::receiveMessages, this);
    std::thread sendThread(&NetworkManager::sendMessages, this);

    // Wait for both threads to finish
    receiveThread.join();
    sendThread.join();

    return roomNumber;
}

void NetworkManager::receiveMessages() {
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

void NetworkManager::sendMessages() {
    std::string message;

    while (true) {
        std::cout << "chat: ";
        std::getline(std::cin, message);
        if (send(clientSocket, message.c_str(), message.size(), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send message." << std::endl;
            break;
        }
    }
}

void NetworkManager::closeConnection() {
    closesocket(clientSocket);
    WSACleanup();
}

NetworkManager::~NetworkManager() {
    closeConnection();
}
