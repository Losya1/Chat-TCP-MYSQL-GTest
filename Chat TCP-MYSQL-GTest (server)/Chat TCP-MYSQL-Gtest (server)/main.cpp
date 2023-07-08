#include "header.h"

int main() {

	//SOCKET//

	in_addr ip_to_num;
	erStat = inet_pton(AF_INET, IP_SERV, &ip_to_num);

	if (erStat <= 0) {
		std::cout << "Error in IP translation to special numeric format" << std::endl;
		return 1;
	}

	WSADATA wsData;

	erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

	if (erStat != 0) {
		std::cout << "Error WinSock version initializaion #";
		std::cout << WSAGetLastError();
		return 1;
	}
	else {
		std::cout << "WinSock initialization is OK" << std::endl;
	}

	SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);

	if (ServSock == INVALID_SOCKET) {
		std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else {
		std::cout << "Server socket initialization is OK" << std::endl;
	}

	sockaddr_in servInfo;
	ZeroMemory(&servInfo, sizeof(servInfo));

	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;
	servInfo.sin_port = htons(PORT_NUM);

	erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));

	if (erStat != 0) {
		std::cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << std::endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else {
		std::cout << "Binding socket to Server info is OK" << std::endl;
	}

	erStat = listen(ServSock, SOMAXCONN);

	if (erStat != 0) {
		std::cout << "Can't start to listen to. Error # " << WSAGetLastError() << std::endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else {
		std::cout << "Listening..." << std::endl;
	}

	sockaddr_in clientInfo;
	ZeroMemory(&clientInfo, sizeof(clientInfo));

	int clientInfo_size = sizeof(clientInfo);

	SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);

	if (ClientConn == INVALID_SOCKET) {
		std::cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
		closesocket(ServSock);
		closesocket(ClientConn);
		WSACleanup();
		return 1;
	}
	else {
		std::cout << "Connection to a client established successfully" << std::endl;
		char clientIP[22];

		inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN);

		std::cout << "Client connected with IP address " << clientIP << std::endl;
	}

	//MYSQL//

	try
	{
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "12345");
	}
	catch (sql::SQLException e)
	{
		std::cout << "Could not connect to server. Error message: " << e.what() << std::endl;
		system("pause");
		exit(1);
	}

	con->setSchema("ChatDB");

	stmt = con->createStatement();

	pstmt = con->prepareStatement("INSERT INTO user(username, password) VALUES(?,?)");

	//MAIN//

	User user;
	std::cout << "Waiting for the message" << std::endl;
	while (true) {
		Client_message.clear();
		recv(ClientConn, Client_message.data(), BUFF_SIZE, 0);

		if (Client_message[0] == '1') {
			user.registration(ClientConn);
		}
	}

	delete stmt;
	delete pstmt;
	delete con;
	delete res;
	closesocket(ServSock);
	closesocket(ClientConn);
	WSACleanup();
	return 0;
}
