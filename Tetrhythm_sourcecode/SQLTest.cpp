#include <mysql.h>
#include <iostream>
#pragma comment(lib, "libmySQL.lib")
using namespace std;
int main() {

	MYSQL Conn;
	MYSQL* ConnPtr = NULL;
	MYSQL_RES* Result;
	MYSQL_ROW Row;
	int Stat;

	mysql_init(&Conn);

	ConnPtr = mysql_real_connect(&Conn, "localhost", "root", "8501", "test", 3306, (char*)NULL, 0);
	if (ConnPtr == NULL) {
		cout << mysql_error(&Conn) << endl;
		return 1;
	}

	const char* Query = "select * from testtable";
	Stat = mysql_query(ConnPtr, Query);
	if (Stat != 0) {
		cout << mysql_error(&Conn) << endl;
		return 1;
	}

	Result = mysql_store_result(ConnPtr);
	while ((Row = mysql_fetch_row(Result)) != NULL) {
		cout << Row[0] << endl;
	}
	mysql_free_result(Result);
	mysql_close(ConnPtr);
	return 0;

}