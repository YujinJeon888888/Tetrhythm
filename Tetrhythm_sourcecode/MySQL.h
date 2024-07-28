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

};
