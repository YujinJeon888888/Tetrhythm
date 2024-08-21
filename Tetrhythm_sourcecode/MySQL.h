#pragma once
#include <mysql.h>
#include <iostream>
#include <string>
#include "UserInfo.h"
#pragma comment(lib, "libmySQL.lib")

class MySQL {
private:
    static const char* host;
    static const char* userName;
    static const char* password;
    static const char* database;

public:
    void insertAndShowUsers(const std::string& str);
    bool isDuflicatedUser(const std::string str);
    bool containsInvalidCharacters(const std::string& str);
    void setLine(std::string ID, int l);
    void setTetris(std::string ID, int t);
    void setHighScore(std::string ID, int s);
    void setCharacterName(std::string ID, std::string c);
    void setPerfectClear(std::string ID, bool b);
    void initUserInfo(std::string ID);
};
