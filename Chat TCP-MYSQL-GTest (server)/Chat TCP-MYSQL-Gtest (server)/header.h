#pragma once
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <vector>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "jdbc/mysql_connection.h"
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

const char IP_SERV[] = "127.0.0.1";
const int PORT_NUM = 7777;
const short BUFF_SIZE = 256;
__declspec(selectany) int erStat;

__declspec(selectany) sql::Driver* driver;
__declspec(selectany) sql::Connection* con;
__declspec(selectany) sql::Statement* stmt;
__declspec(selectany) sql::PreparedStatement* pstmt;
__declspec(selectany) sql::ResultSet* res;

class Server_functions {
public:
	void message_distributor(SOCKET ClientConn);
	void registration(SOCKET ClientSock);
};