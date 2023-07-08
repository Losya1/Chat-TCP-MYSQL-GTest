#pragma once
#include <iostream>
#include <vector>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include "jdbc/mysql_connection.h"
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

#pragma comment(lib, "Ws2_32.lib")

const char IP_SERV[] = "127.0.0.1";
const int PORT_NUM = 7777;
const short BUFF_SIZE = 256;
__declspec(selectany) int erStat;

__declspec(selectany) sql::Driver* driver;
__declspec(selectany) sql::Connection* con;
__declspec(selectany) sql::Statement* stmt;
__declspec(selectany) sql::PreparedStatement* pstmt;
__declspec(selectany) sql::ResultSet* res;

__declspec(selectany) std::vector <char> Client_message(BUFF_SIZE), Server_message(BUFF_SIZE);

class User {
public:
	void registration(SOCKET ClientSock);
};
