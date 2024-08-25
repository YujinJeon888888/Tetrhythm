#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <thread>
#include "SceneManager.h"
#include "tetrisScene.h"
#include <conio.h>
#include "Well.h"


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


    //void receiveData(bool data[Well::Width][Well::Height]);

  //  void receiveData(bool data[Well::Width][Well::Height]);

public:
    // Get the singleton instance
    static Multi* getInstance();
    static void resetInstance() {
        // 기존 인스턴스 삭제
        if (instance) {
           
            delete instance;
            instance = nullptr;
        }
        // 새로운 인스턴스 생성
        instance = new Multi();
    }
    void sendID(std::string id, std::string charImageStr);
    std::pair<std::string, std::string> receiveIDAndCharacter();
    std::string receiveOpponentData();
    void sendData(bool data[10][20], const Tetromino::Type dataTypes[Well::Width][Well::Height], int line, int tetris);
    void sendMessage(int type);
    void sendGameOver();
    void sendHeartInfo(std::string msg);
    int receiveHeartData();
    int receiveMessegeData();
    int receiveData(std::array<std::array<bool, 20>, 10>& data, Tetromino::Type(&dataTypes)[Well::Width][Well::Height]);

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
    bool isClear = false;
    bool isReady = false;
    bool hasCode = false;
    int opponentLine = 0;
    int opponentTetris = 0;

};

