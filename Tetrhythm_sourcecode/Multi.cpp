
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Multi.h"

Multi* Multi::instance = nullptr;
Multi::Multi() {

    //"52.14.83.66";//
    addr = "52.14.83.66";// "127.0.0.1";
    //"52.14.83.66"
    //getRandomRoom();
    //WSADATA wsaData;
    //WSAStartup(MAKEWORD(2, 2), &wsaData);

    //clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //sockaddr_in serverAddr;
    //serverAddr.sin_family = AF_INET;
    //serverAddr.sin_addr.s_addr = inet_addr(addr.c_str());
    //serverAddr.sin_port = htons(8080);

    //std::cout << "connect to server";
    //connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
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

    // 메시지 타입 전송
    if (send(clientSocket, id.c_str(), sizeof(id), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message type for id" << std::endl;
        return;
    }

    // 메시지 타입 전송
    if (send(clientSocket, charImageStr.c_str(), sizeof(charImageStr), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message type for character." << std::endl;
        return;
    }

}

std::string Multi::receiveOpponentData() {
    char buffer[1024];

    // fd_set과 timeout 설정
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(clientSocket, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 16000;  // 16ms timeout (예: 60 FPS)

    // select로 소켓의 상태를 확인
    int activity = select(clientSocket + 1, &readfds, NULL, NULL, &timeout);

    if (activity > 0 && FD_ISSET(clientSocket, &readfds)) {
        // 데이터 수신
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        // 수신된 바이트가 0보다 큰 경우에만 처리
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';  // Null-terminate the buffer
            return std::string(buffer);    // 문자열로 반환
        }
        // 연결이 종료된 경우
        else if (bytesReceived == 0) {
          //  std::cerr << "Connection closed by the peer" << std::endl;
            return "CONNECTION_CLOSED";    // 연결 종료 시 명시적 문자열 반환
        }
        // 오류가 발생한 경우
        else {
           // std::cerr << "Receive error: " << strerror(errno) << std::endl;
            return "ERROR";    // 오류 시 명시적 문자열 반환
        }
    }

    // 소켓 상태가 변경되지 않은 경우
    return "NO_DATA";    // 데이터가 없음을 명시하는 문자열 반환
}

void Multi::sendData(bool data[10][20], const Tetromino::Type dataTypes[Well::Width][Well::Height]) {

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
    
}

void Multi::sendGameOver() {

    char messageType = 2;

    // 메시지 타입 전송
    if (send(clientSocket, &messageType, sizeof(messageType), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send message type for game clear." << std::endl;
        return;
    }

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

        return messageType;
        
    }
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
                        return 3;  // 성공적으로 데이터 수신 및 변환 완료
                    }

                }
            }
            else if (messageType == 2) {
                char Message[256];  // 메시지 저장 버퍼

                return 2;
               // bytesReceived = recv(clientSocket, Message, sizeof(Message), 0);
                if (bytesReceived > 0) {
                    // 게임 클리어 메시지 출력
                
                    // 게임 클리어 관련 처리
                    if (memcmp(buffer, "Game Clear", strlen("Game Clear")) == 0) {
                       
                        std::cout << Message << std::endl;
                        // std::cout << "The game is ready\n";
                        //한번 더 체크 해야함.
                        isClear = true;
                        
                    }
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

// Create threads for receiving and sending messages
//  std::thread receiveThread(&Multi::receiveMessages, this);
//  std::thread sendThread(&Multi::sendMessages, this);

// Wait for both threads to finish
//   receiveThread.join();
//  sendThread.join();


// closesocket(clientSocket);
// WSACleanup();
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
