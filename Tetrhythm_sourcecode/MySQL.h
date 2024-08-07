#pragma once
#include <mysql.h>
#include <iostream>
#include <string>
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
    int getLine(std::string ID) const;
    void setTetris(std::string ID, int t);
    int getTetris(std::string ID)const;
};
