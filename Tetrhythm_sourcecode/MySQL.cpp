#include "MySQL.h"

// 클래스 외부에서 초기화
const char* MySQL::host = "iu51mf0q32fkhfpl.cbetxkdyhwsb.us-east-1.rds.amazonaws.com";
const char* MySQL::userName = "c4soupya4kvutpg2";
const char* MySQL::password = "nyryixy9qhnhzfze";
const char* MySQL::database = "ljk5c9ot0z8wofa5";

void MySQL::insertAndShowUsers(const std::string& str) {
    MYSQL Conn;
    MYSQL* ConnPtr = NULL;
    MYSQL_RES* Result;
    MYSQL_ROW Row;
    int Stat;

    mysql_init(&Conn);

    ConnPtr = mysql_real_connect(&Conn, host, userName, password, database, 3306, (char*)NULL, 0);
    if (ConnPtr == NULL) {
        std::cout << mysql_error(&Conn) << std::endl;
        exit(-1);
    }

    // 값 삽입 쿼리 실행
    std::string insertQuery = "INSERT INTO Users (name) VALUES ('" + str + "')";
    const char* _insertQuery = insertQuery.c_str();
    Stat = mysql_query(ConnPtr, _insertQuery);
    if (Stat != 0) {
        std::cout << mysql_error(&Conn) << std::endl;
        exit(-1);
    }

    // 테이블 상태 출력 쿼리 실행
    const char* selectQuery = "SELECT * FROM Users";
    Stat = mysql_query(ConnPtr, selectQuery);
    if (Stat != 0) {
        std::cout << mysql_error(&Conn) << std::endl;
        exit(-1);
    }

    // 결과를 가져와서 출력
    Result = mysql_store_result(ConnPtr);
    int numFields = mysql_num_fields(Result);

    while ((Row = mysql_fetch_row(Result)) != NULL) {
        for (int i = 0; i < numFields; i++) {
            std::cout << (Row[i] ? Row[i] : "NULL") << " ";
        }
        std::cout << std::endl;
    }

    mysql_free_result(Result);
    mysql_close(ConnPtr);
}

bool MySQL::isDuflicatedUser(const std::string str)
{
    MYSQL Conn;
    MYSQL* ConnPtr = NULL;
    MYSQL_RES* Result;
    MYSQL_ROW Row;
    int Stat;

    mysql_init(&Conn);

    ConnPtr = mysql_real_connect(&Conn, host, userName, password, database, 3306, (char*)NULL, 0);
    if (ConnPtr == NULL) {
        std::cout << mysql_error(&Conn) << std::endl;
        exit(-1);
    }

    // 테이블 상태 출력 쿼리 실행
    const char* selectQuery = "SELECT name FROM Users";
    Stat = mysql_query(ConnPtr, selectQuery);
    if (Stat != 0) {
        std::cout << mysql_error(&Conn) << std::endl;
        exit(-1);
    }

    // 결과를 가져와서 출력
    Result = mysql_store_result(ConnPtr);
    int numFields = mysql_num_fields(Result);

    while ((Row = mysql_fetch_row(Result)) != NULL) {
        for (int i = 0; i < numFields; i++) {
            if (Row[i] == str) {
                return true;
            }
        }
        std::cout << std::endl;
    }

    mysql_free_result(Result);
    mysql_close(ConnPtr);

    return false;
}

bool MySQL::containsInvalidCharacters(const std::string& str) {
    return std::find_if(str.begin(), str.end(), [](unsigned char c) {
        return !(std::isalnum(c)); // 영문자 또는 숫자가 아니면 허용되지 않는 문자
        }) != str.end();
}

void MySQL::setLine(std::string ID, int l) {
    MYSQL Conn;
    MYSQL* ConnPtr = NULL;
    int Stat;

    mysql_init(&Conn);

    ConnPtr = mysql_real_connect(&Conn, host, userName, password, database, 3306, (char*)NULL, 0);
    if (ConnPtr == NULL) {
        std::cout << mysql_error(&Conn) << std::endl;
        exit(-1);
    }

    // 값 삽입 쿼리 실행
    int newLine = getLine(ID) + l;
    std::string updateQuery = "update Users set line = " + std::to_string(newLine) + " where name = '" + ID + "'";
    Stat = mysql_query(ConnPtr, updateQuery.c_str());
    if (Stat != 0) {
        std::cout << "Update Error: " << mysql_error(&Conn) << std::endl;
        mysql_close(ConnPtr);
        exit(-1);
    }

    mysql_close(ConnPtr);
}

int MySQL::getLine(std::string ID) const {
    MYSQL Conn;
    MYSQL* ConnPtr = NULL;
    MYSQL_RES* Result;
    MYSQL_ROW Row;
    int Stat;

    mysql_init(&Conn);

    ConnPtr = mysql_real_connect(&Conn, host, userName, password, database, 3306, (char*)NULL, 0);
    if (ConnPtr == NULL) {
        std::cout << mysql_error(&Conn) << std::endl;
        exit(-1);
    }

    // 테이블 상태 출력 쿼리 실행
    std::string query = "SELECT line FROM Users where name = '" + ID + "'";
    const char* selectQuery = query.c_str();
    Stat = mysql_query(ConnPtr, selectQuery);
    if (Stat != 0) {
        std::cout << mysql_error(&Conn) << std::endl;
        exit(-1);
    }

    // 결과를 가져와서 출력
    Result = mysql_store_result(ConnPtr);
    int numFields = mysql_num_fields(Result);

    while ((Row = mysql_fetch_row(Result)) != NULL) {
        for (int i = 0; i < numFields; i++) {
            std::cout << (Row[i] ? Row[i] : "NULL") << " ";
            
            return std::stoi(Row[0]);
        }
        std::cout << std::endl;
    }

    mysql_free_result(Result);
    mysql_close(ConnPtr);
}
void MySQL::setTetris(std::string ID, int t) {
    MYSQL Conn;
    MYSQL* ConnPtr = NULL;
    int Stat;

    mysql_init(&Conn);

    ConnPtr = mysql_real_connect(&Conn, host, userName, password, database, 3306, (char*)NULL, 0);
    if (ConnPtr == NULL) {
        std::cout << mysql_error(&Conn) << std::endl;
        exit(-1);
    }

    // 값 삽입 쿼리 실행
    int newTetris = getTetris(ID) + t;
    std::string updateQuery = "update Users set tetris = " + std::to_string(newTetris) + " where name = '" + ID + "'";
    Stat = mysql_query(ConnPtr, updateQuery.c_str());
    if (Stat != 0) {
        std::cout << "Update Error: " << mysql_error(&Conn) << std::endl;
        mysql_close(ConnPtr);
        exit(-1);
    }

    mysql_close(ConnPtr);
}
int MySQL::getTetris(std::string ID) const {
    MYSQL Conn;
    MYSQL* ConnPtr = NULL;
    MYSQL_RES* Result;
    MYSQL_ROW Row;
    int Stat;

    mysql_init(&Conn);

    ConnPtr = mysql_real_connect(&Conn, host, userName, password, database, 3306, (char*)NULL, 0);
    if (ConnPtr == NULL) {
        std::cout << mysql_error(&Conn) << std::endl;
        exit(-1);
    }

    // 테이블 상태 출력 쿼리 실행
    std::string query = "SELECT tetris FROM Users where name = '" + ID + "'";
    const char* selectQuery = query.c_str();
    Stat = mysql_query(ConnPtr, selectQuery);
    if (Stat != 0) {
        std::cout << mysql_error(&Conn) << std::endl;
        exit(-1);
    }

    // 결과를 가져와서 출력
    Result = mysql_store_result(ConnPtr);
    int numFields = mysql_num_fields(Result);

    while ((Row = mysql_fetch_row(Result)) != NULL) {
        for (int i = 0; i < numFields; i++) {
            std::cout << (Row[i] ? Row[i] : "NULL") << " ";

            return std::stoi(Row[0]);
        }
        std::cout << std::endl;
    }

    mysql_free_result(Result);
    mysql_close(ConnPtr);
}