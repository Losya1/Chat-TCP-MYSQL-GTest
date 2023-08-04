#pragma once
#include <iostream>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>

#pragma comment(lib, "ws2_32.lib")

const char SERVER_IP[] = "127.0.0.1";
const short SERVER_PORT_NUM = 7777;
const short BUFF_SIZE = 256;

class User {
private:
	std::vector<std::pair<std::string, std::string>> user_arr = {};
	std::vector <std::string> mes_arr;
public:
	void registration(SOCKET ClientSock);
	std::string login(SOCKET ClientSock);
	void authorized_user(const std::string name, SOCKET ClientSock);
};
