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
