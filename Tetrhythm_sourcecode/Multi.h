#pragma once

#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <thread>
#include "SceneManager.h"
#include "tetrisScene.h"
#include <conio.h>

class Multi{

private:
    SOCKET clientSocket;
    static Multi* instance;

    // Private constructor to prevent instantiation
    Multi();

    void connectServer();

    // Setup connection with the server
    void setupConnection();

    // Receive room number from the server
    int receiveRoomNumber();

public:
    // Get the singleton instance
    static Multi* getInstance();

    // Get a random room and start communication
    int getRandomRoom();

    
    int createRoom();

    std::string addr;
    int roomCode = 0;
    int joinRoom(std::string password);

    void connetOpponent();

    // Receive messages from the server
    void receiveMessages();

    // Send messages to the server
    void sendMessages();

    // Close the socket connection
    void closeConnection();

    // Destructor
    ~Multi();

    bool isReady = false;
    bool hasCode = false;

};

