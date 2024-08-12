#pragma once

#include <string>
#include <iostream>
#include <thread>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

class NetworkManager {

private:
    SOCKET clientSocket;
    static NetworkManager* instance;
    

    // Private constructor to prevent instantiation
    NetworkManager();

    // Setup connection with the server
    void setupConnection();

    // Receive room number from the server
    int receiveRoomNumber();

public:
    // Get the singleton instance
    static NetworkManager* getInstance();

    // Get a random room and start communication
    int getRandomRoom();

    // Receive messages from the server
    void receiveMessages();

    // Send messages to the server
    void sendMessages();

    // Close the socket connection
    void closeConnection();

    // Destructor
    ~NetworkManager();
};


