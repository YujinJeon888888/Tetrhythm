#include <mysql.h>

#pragma comment(lib, "libmySQL.lib")
#include <iostream>
using namespace std;
int main() {
	cout << mysql_get_client_info() << endl;
	return 0;
}