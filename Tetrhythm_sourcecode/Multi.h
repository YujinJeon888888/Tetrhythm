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
#include "Tetromino.h"


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
    void sendTetromino(Tetromino& tetromino);
    void sendNextTetrominos(const std::array<Tetromino, 3>& tetrominoArray);
    void sendData(bool data[10][20], const Tetromino::Type dataTypes[Well::Width][Well::Height], int line, int tetris);
    void sendScore(int Score);
    void sendMessage(int type);
    void sendGameOver();
    void sendHeartInfo(std::string msg);
    void receiveTetromino();
    int receiveHeartData();
    int receiveMessegeData();
    int recevType3Data();
    int receiveData(std::array<std::array<bool, 20>, 10>& data, Tetromino::Type(&dataTypes)[Well::Width][Well::Height]);
    void receiveTetrominos();
    int receiveScore();

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
    bool hasData = false;
    bool hasTetromino = false;
    int opponentLine = 0;
    int opponentTetris = 0;
    int opponentScore = 0;

    std::array<std::array<bool, 20>, 10> data ;
    Tetromino::Type dataTypes[10][20] ;
    Tetromino tetromino;
    std::array<Tetromino, 3> nextTetrominos;
};

