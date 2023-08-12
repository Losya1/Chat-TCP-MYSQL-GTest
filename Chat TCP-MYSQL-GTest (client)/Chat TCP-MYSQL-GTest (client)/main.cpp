#include "header.h"

int main() {
	int erStat;		// For checking errors in sockets functions

	//IP in string format to numeric format for socket functions. Data is in "ip_to_num"
	in_addr ip_to_num;
	inet_pton(AF_INET, SERVER_IP, &ip_to_num);

	// WinSock initialization
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

	// Socket initialization
	SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, 0);

	if (ClientSock == INVALID_SOCKET) {
		std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
		closesocket(ClientSock);
		WSACleanup();
	}
	else {
		std::cout << "Client socket initialization is OK" << std::endl;
	}

	// Establishing a connection to Server
	sockaddr_in servInfo;

	ZeroMemory(&servInfo, sizeof(servInfo));

	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;
	servInfo.sin_port = htons(SERVER_PORT_NUM);

	erStat = connect(ClientSock, (sockaddr*)&servInfo, sizeof(servInfo));

	if (erStat != 0) {
		std::cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << std::endl;
		closesocket(ClientSock);
		WSACleanup();
		return 1;
	}
	else {
		std::cout << "Connection established SUCCESSFULLY. Ready to send a message to Server" << std::endl;
	}

	std::vector <char> Arr(BUFF_SIZE);

	User chat;
	bool b = true;
	char x;

	while (b == true) {
		std::cout << "Enter 1 to registr, 2 to login or 3 to quit." << std::endl;
		std::cin >> x;
		switch (x) {
		case('1'):
			chat.registration(ClientSock);
			break;
		case('2'):
			chat.authorized_user(chat.login(ClientSock), ClientSock);
			break;
		case('3'):
			Arr.insert(Arr.begin(), '7');
			send(ClientSock, Arr.data(), BUFF_SIZE, 0);
			recv(ClientSock, Arr.data(), BUFF_SIZE, 0);
			b = false;
			break;
		default:
			std::cout << "error" << std::endl;
			break;
		}
	}
	closesocket(ClientSock);
	WSACleanup();
	return 0;
}
