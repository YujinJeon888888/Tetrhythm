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

	ConnPtr = mysql_real_connect(&Conn, "iu51mf0q32fkhfpl.cbetxkdyhwsb.us-east-1.rds.amazonaws.com", "c4soupya4kvutpg2", "nyryixy9qhnhzfze", "ljk5c9ot0z8wofa5", 3306, (char*)NULL, 0);
	if (ConnPtr == NULL) {
		cout << mysql_error(&Conn) << endl;
		return 1;
	}

	const char* Query = "SELECT * from test_table";
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