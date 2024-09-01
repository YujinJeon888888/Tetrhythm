
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Multi.h"

Multi* Multi::instance = nullptr;
Multi::Multi() : tetromino(61, 100) {

     addr = "13.125.65.119";
   //addr = "127.0.0.1";
  
}
              
void Multi::connectServer() {

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(addr.c_str());
    serverAddr.sin_port = htons(8080);

    std::cout << "connect to server";
    connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
}

Multi* Multi::getInstance() {
    if (instance == nullptr) {
        instance = new Multi();
    }
    return instance;
}


void Multi::sendID(std::string id, std::string charImageStr) {
    char messageType = '9';

    // 메시지 타입 전송
    if (send(clientSocket, &messageType, sizeof(messageType), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message type for message type of ID." << std::endl;
        return;
    }

    // ID 길이 전송
    int idLength = id.size();
    if (send(clientSocket, reinterpret_cast<char*>(&idLength), sizeof(idLength), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send ID length." << std::endl;
        return;
    }

    // ID 데이터 전송
    if (send(clientSocket, id.c_str(), idLength, 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send ID data." << std::endl;
        return;
    }

    // 캐릭터 이미지 문자열 길이 전송
    int charImageStrLength = charImageStr.size();
    if (send(clientSocket, reinterpret_cast<char*>(&charImageStrLength), sizeof(charImageStrLength), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send character image string length." << std::endl;
        return;
    }
    //std::cout << reinterpret_cast<char*>(&charImageStrLength);
    // 캐릭터 이미지 문자열 데이터 전송
    if (send(clientSocket, charImageStr.c_str(), charImageStrLength, 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send character image string data." << std::endl;
        return;
    }
}



void Multi::sendTetromino(Tetromino& tetromino) {

    char messageType = '7';
    char data[sizeof(Tetromino)];
   
        if (send(clientSocket, &messageType, sizeof(messageType), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send message type for message type of tetromino data." << std::endl;
            return;
        }
 
    tetromino.serialize(data);

    if (send(clientSocket, data, sizeof(data), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send Tetromino data." << std::endl;
    }
}
void Multi::sendNextTetrominos(const std::array<Tetromino, 3>& tetrominoArray) {

    char messageType = 8;
    char data[sizeof(Tetromino) * 3];
    for (size_t i = 0; i < tetrominoArray.size(); ++i) {
        tetrominoArray[i].serialize(data + i * sizeof(Tetromino));
    }
    if (send(clientSocket, &messageType, sizeof(messageType), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message type for message type of tetromino data." << std::endl;
        return;
    }

    if (send(clientSocket, data, sizeof(data), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send Tetromino data." << std::endl;
    }
}


void Multi::sendData(bool data[10][20], const Tetromino::Type dataTypes[Well::Width][Well::Height],int line,int tetris) {

    char messageType = 3;

    // 직렬화: bool 배열을 char 배열로 변환 (bool은 1바이트)
    char buffer[Well::Width * Well::Height];

    for (int i = 0; i < Well::Width; ++i) {
       for (int j = 0; j < Well::Height; ++j) {
                buffer[i * Well::Height + j] = static_cast<char>(data[i][j]);
       //std::cout << static_cast<int>(buffer[i * Well::Height + j]);
       }
       // std::cout << std::endl;
    }

    char typeBuffer[Well::Width * Well::Height];
    for (int i = 0; i < Well::Width; ++i) {
        for (int j = 0; j < Well::Height; ++j) {
                typeBuffer[i * Well::Height + j] = static_cast<char>(dataTypes[i][j]);
        }
    }

    if (send(clientSocket, &messageType, sizeof(messageType), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message type for block data." << std::endl;
        return;
    }

    // 3. 데이터 전송 (두 배열을 순서대로 전송)
    if (send(clientSocket, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send bool data." << std::endl;
    }
    if (send(clientSocket, typeBuffer, sizeof(typeBuffer), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send type data." << std::endl;
    }

    int networkINT= line;

    // 데이터를 전송합니다.

    if (send(clientSocket, (char*)&networkINT, sizeof(networkINT), 0) == SOCKET_ERROR) {
            std::cerr << "Failed to send type data." << std::endl;
    }
    networkINT = tetris;
  
    if (send(clientSocket, (char*)&networkINT, sizeof(networkINT), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send type data." << std::endl;
    }
  
    
}


void Multi::sendScore(int Score) {
    char messageType = 6;

    // 메시지 타입 전송
    if (send(clientSocket, &messageType, sizeof(messageType), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message type for message" << std::endl;
        return;
    }

    int networkINT = Score;

    // 데이터를 전송합니다.

      if (send(clientSocket, (char*)&networkINT, sizeof(networkINT), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send score data." << std::endl;
    }
}


void Multi::sendMessage(int type) {

    char messageType = type;

    // 메시지 타입 전송
    if (send(clientSocket, &messageType, sizeof(messageType), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message type for message" << std::endl;
        return;
    }

}


void Multi::sendGameOver() {

    char messageType = 2;

    // 메시지 타입 전송
    if (send(clientSocket, &messageType, sizeof(messageType), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message type for game over/clear." << std::endl;
        return;
    }

  

}

void Multi::sendHeartInfo(std::string msg) {

    char messageType = 4;
    
    if (msg == "plus") messageType = 5;

    // 메시지 타입 전송
    if (send(clientSocket, &messageType, sizeof(messageType), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message type for heart." << std::endl;
        return;
    }

   /* if (send(clientSocket, msg.c_str(), sizeof(msg), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message type for msg." << std::endl;
        return;
    }*/

}


int Multi::receiveMessegeData() {
    char buffer[Well::Width * Well::Height];
    char typeBuffer[Well::Width * Well::Height];
    char messageType;
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(clientSocket, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 16000;  // 16ms timeout (예: 60 FPS)

    int activity = select(clientSocket + 1, &readfds, NULL, NULL, &timeout);

    if (activity > 0 && FD_ISSET(clientSocket, &readfds)) {
        char buffer[Well::Width * Well::Height];

        // 메시지 타입 수신 (1바이트)
        int bytesReceived = recv(clientSocket, &messageType, sizeof(messageType), 0);

        if (bytesReceived <= 0) return false;

        if (static_cast<int>(messageType) == 3) {
          return recevType3Data();
        }
        else if (messageType == 6) {

             int networkInt;
            int bytesReceived = recv(clientSocket, (char*)&networkInt, sizeof(networkInt), 0);

            if (bytesReceived > 0) {

              //  int score = ntohl(networkInt);

                opponentScore = networkInt;
            

                return messageType;

            }

        } 
        else if (messageType == '7') {

            receiveTetromino();
            return 7;
        }
        else if (messageType == 8) {
            receiveTetrominos();
        }
        else if (messageType == '9') {
            receiveIDAndCharacter();
            std::cout << 9;
            return 9;
        }
        else if (static_cast<int>(messageType) != 0)
            std::cout << "Received message type: " << static_cast<int>(messageType) << std::endl;

        return static_cast<int>(messageType);
        
    }
}

void Multi::receiveIDAndCharacter() {

    int idLength = 0;

    int bytesReceived = recv(clientSocket, reinterpret_cast<char*>(&idLength), sizeof(idLength), 0);
    if (bytesReceived <= 0) {
        std::cerr << "Failed to receive ID or connection closed" << std::endl;
        return;
    }


    std::vector<char> idBuffer(idLength + 1);
    // 첫 번째 데이터 (ID) 수신
    bytesReceived = recv(clientSocket, idBuffer.data(), idLength, 0);
    if (bytesReceived <= 0) {
        std::cerr << "Failed to receive ID or connection closed" << std::endl;
        return;
    }
    idBuffer[idLength] = '\0'; // NULL 종료
    opponentId = std::string(idBuffer.data());
    int charImageStrLength = 0;

    bytesReceived = recv(clientSocket, reinterpret_cast<char*>(&charImageStrLength), sizeof(charImageStrLength), 0);
    if (bytesReceived <= 0) {
        std::cerr << "Failed to receive ID or connection closed" << std::endl;
        return;
    }

    std::vector<char> charImageStrBuffer(charImageStrLength + 1);
  
    // 두 번째 데이터 (캐릭터 이미지 경로) 수신
    bytesReceived = recv(clientSocket, charImageStrBuffer.data(), charImageStrLength, 0);
    if (bytesReceived <= 0) {
        std::cerr << "Failed to receive character image path or connection closed" << std::endl;
        return;
    }
    charImageStrBuffer[charImageStrLength] = '\0'; // NULL 종료
    opponentCharacter = std::string(charImageStrBuffer.data());

    return;

}


void Multi::receiveTetrominos() {
    char data[sizeof(Tetromino) * 3];
    int receivedBytes = recv(clientSocket, data, sizeof(data), 0);

    if (receivedBytes == SOCKET_ERROR) {
        std::cerr << "Failed to receive Tetromino array." << std::endl;
    }
    else if (receivedBytes == sizeof(data)) {
        for (size_t i = 0; i < nextTetrominos.size(); ++i) {
            nextTetrominos[i].deserialize(data + i * sizeof(Tetromino));
         
        }
    }
    else {
        std::cerr << "Received partial Tetromino array data." << std::endl;
    }
}
void Multi::receiveTetromino() {
    char data[sizeof(Tetromino)];
    int receivedBytes = recv(clientSocket, data, sizeof(data), 0);

    if (receivedBytes == SOCKET_ERROR) {
        std::cerr << "Failed to receive Tetromino data." << std::endl;
    }
    else if (receivedBytes >0) {
        tetromino.deserialize(data);
           
        hasTetromino = true;
    }
    else {
        std::cerr << "Received partial Tetromino data." << std::endl;
    }
}

int Multi::recevType3Data() {

    char buffer[Well::Width * Well::Height];
    char typeBuffer[Well::Width * Well::Height];

    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        // 역직렬화: char 배열을 bool 배열로 변환
        for (int i = 0; i < Well::Width; ++i) {
            for (int j = 0; j < Well::Height; ++j) {
                data[i][j] = static_cast<bool>(buffer[i * Well::Height + j]);
            }
        }

        bytesReceived = recv(clientSocket, typeBuffer, sizeof(typeBuffer), 0);

        if (bytesReceived > 0) {
            // Tetromino::Type 배열로 역직렬화
            for (int i = 0; i < Well::Width; ++i) {
                for (int j = 0; j < Well::Height; ++j) {
                    dataTypes[i][j] = static_cast<Tetromino::Type>(typeBuffer[i * Well::Height + j]);
                }
            }
            int networkInt;
            bytesReceived = recv(clientSocket, (char*)&networkInt, sizeof(networkInt), 0);

            if (bytesReceived > 0) {
                opponentLine = networkInt;


                bytesReceived = recv(clientSocket, (char*)&networkInt, sizeof(networkInt), 0);

                if (bytesReceived > 0) {
                    opponentTetris = networkInt;
                    hasData = true;
                    return 3;
                }
            }

        }

    }

    return false;
}

int Multi::receiveData(std::array<std::array<bool, 20>, 10>& data, Tetromino::Type(&dataTypes)[Well::Width][Well::Height]) {
   
        char buffer[Well::Width * Well::Height];
        char typeBuffer[Well::Width * Well::Height];
        char messageType;
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(clientSocket, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 16000;  // 16ms timeout (예: 60 FPS)

        int activity = select(clientSocket + 1, &readfds, NULL, NULL, &timeout);

        if (activity > 0 && FD_ISSET(clientSocket, &readfds)) {
            char buffer[Well::Width * Well::Height];
            
            // 메시지 타입 수신 (1바이트)
            int bytesReceived = recv(clientSocket, &messageType, sizeof(messageType), 0);

            if (bytesReceived <= 0) return false;
            if (messageType == 3) {


                int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
                if (bytesReceived > 0) {
                    // 역직렬화: char 배열을 bool 배열로 변환
                    for (int i = 0; i < Well::Width; ++i) {
                        for (int j = 0; j < Well::Height; ++j) {
                            data[i][j] = static_cast<bool>(buffer[i * Well::Height + j]);
                        }
                    }

                     bytesReceived = recv(clientSocket, typeBuffer, sizeof(typeBuffer), 0);

                    if (bytesReceived > 0) {
                        // Tetromino::Type 배열로 역직렬화
                        for (int i = 0; i < Well::Width; ++i) {
                            for (int j = 0; j < Well::Height; ++j) {
                                dataTypes[i][j] = static_cast<Tetromino::Type>(typeBuffer[i * Well::Height + j]);
                            }
                        }
                        char networkInt;
                        bytesReceived = recv(clientSocket, (char*)&networkInt, sizeof(networkInt), 0);

                        if (bytesReceived > 0) {
                            opponentLine = networkInt;

                    
                            bytesReceived = recv(clientSocket, (char*)&networkInt, sizeof(networkInt), 0);

                            if (bytesReceived > 0) {
                                opponentTetris = networkInt;
                                return messageType;  // 성공적으로 데이터 수신 및 변환 완료
                            }
                        }
                      
                    }

                }
            }
            else if (messageType == 2) {
                
                std::cout << "type 2!";
                return 2;
             
            }
            
            else {
                return messageType;
            }
          
        }
       
        return false;
}


int Multi::receiveScore() {

    char messageType;
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(clientSocket, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 16000;  // 16ms timeout (예: 60 FPS)

    int activity = select(clientSocket + 1, &readfds, NULL, NULL, &timeout);

    if (activity > 0 && FD_ISSET(clientSocket, &readfds)) {

        // 메시지 타입 수신 (1바이트)
        int bytesReceived = recv(clientSocket, &messageType, sizeof(messageType), 0);

        if (bytesReceived <= 0) return false;
        if (messageType == 6) {

            char networkInt;
            int bytesReceived = recv(clientSocket, &networkInt, sizeof(networkInt), 0);
           
                if (bytesReceived > 0) {
                 
                    
                   
                  opponentScore = networkInt;
                  std::cout << opponentScore;

                  return true;
                    

                }

        }
      

    }

    return false;
}

int Multi::getRandomRoom() {

    connectServer();
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
    connectServer();
    
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
    roomCode = roomNumber;

    connetOpponent();

    return roomNumber;
}

int Multi::joinRoom(std::string password) {
    connectServer();
    std::string msg = "join_room";
    if (send(clientSocket, msg.c_str(), msg.size(), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message." << std::endl;

        return -1;
    }

    if (send(clientSocket, password.c_str(), password.size(), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message." << std::endl;
        return -1;
    }

    std::cout << "try join room " << password << std::endl;

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
    shutdown(clientSocket, SD_BOTH);  // 전송 종료
    closesocket(clientSocket);
    WSACleanup();
  
}

Multi::~Multi() {

    std::cout << "exit";
    send(clientSocket, "exit", strlen("exit"), 0);
   // closeConnection();
}
