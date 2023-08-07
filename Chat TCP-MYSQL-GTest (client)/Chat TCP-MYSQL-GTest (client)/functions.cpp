#include "header.h"

std::vector <char> Username(BUFF_SIZE), Password(BUFF_SIZE), Server_message(BUFF_SIZE), Client_message(BUFF_SIZE);

void User::registration(SOCKET ClientSock) {
	std::cout << "Enter username" << std::endl;
	std::cin >> Username.data();
	Username.insert(Username.begin(), '1');
	send(ClientSock, Username.data(), BUFF_SIZE, 0);

	recv(ClientSock, Server_message.data(), BUFF_SIZE, 0);
	if (Server_message[0] == 'r') {								//r - error
		std::cout << "This user already exists" << std::endl;
		Client_message[0] = 'r';
		send(ClientSock, Client_message.data(), BUFF_SIZE, 0);
		return;
	}

	if (Server_message[0] == 'k') {
		std::cout << "Enter password" << std::endl;
		std::cin >> Password.data();
		send(ClientSock, Password.data(), BUFF_SIZE, 0);

		recv(ClientSock, Server_message.data(), BUFF_SIZE, 0);

		if (Server_message[0] == 'k') {								//k - ok
			std::cout << "Registration complete" << std::endl;
		}
		else {
			std::cout << "Registration Error #1" << std::endl;
			return;
		}
	}
	else {
		std::cout << "Error" << std::endl;
		return;
	}
}

std::string User::login(SOCKET ClientSock) {
	std::cout << "Enter username" << std::endl;
	std::cin >> Username.data();
	Username.insert(Username.begin(), '2');
	send(ClientSock, Username.data(), BUFF_SIZE, 0);

	recv(ClientSock, Server_message.data(), BUFF_SIZE, 0);
	if (Server_message[0] == 'r') {
		std::cout << "There is no such user" << std::endl;
		Server_message[0] = 'r';
		send(ClientSock, Server_message.data(), BUFF_SIZE, 0);
		return "Error";
	}

	if (Server_message[0] == 'k') {
		std::cout << "Enter password" << std::endl;
		std::cin >> Password.data();
		send(ClientSock, Password.data(), BUFF_SIZE, 0);

		recv(ClientSock, Server_message.data(), BUFF_SIZE, 0);
		if (Server_message[0] == 'r') {
			std::cout << "Invalid password" << std::endl;
			Client_message[0] = 'r';
			send(ClientSock, Client_message.data(), BUFF_SIZE, 0);
			return "Error";
		}
		if (Server_message[0] == 'k') {
			Username.erase(Username.begin());
			return Username.data();
		}
		else {
			std::cout << "Error" << std::endl;
			return "Error";
		}
	}
	else {
		std::cout << "Error" << std::endl;
		return "Error";
	}
}

void User::authorized_user(const std::string name, SOCKET ClientSock) {
	if (name == "Error") {
		return;
	}
	std::cout << "Hello " << name << std::endl
		<< "Enter 1 to send a message, 2 to send a message to someone, 3 to show messages or 4 to logout." << std::endl;
	bool b = true;
	char x;
	std::string write_name, message;
	short cmes = 0;
	while (b == true) {
		std::cin >> x;
		switch (x) {
		case('1'):
			while (1) {
				std::cout << "Write message" << std::endl;
				std::cin >> Client_message.data();
				Client_message.insert(Client_message.begin(), '3');

				send(ClientSock, Client_message.data(), BUFF_SIZE, 0);

				recv(ClientSock, Server_message.data(), BUFF_SIZE, 0);

				std::cout << name << ": " << Server_message.data() << std::endl;
				break;
		case('2'):
			std::cout << "Write the name of the person to send the message to." << std::endl;
			std::cin >> Client_message.data();
			Client_message.insert(Client_message.begin(), '4');
			send(ClientSock, Client_message.data(), BUFF_SIZE, 0);

			recv(ClientSock, Server_message.data(), BUFF_SIZE, 0);
			if (Server_message[0] == 'r') {
				std::cout << "There is no such user" << std::endl;
				break;
			}
			if (Server_message[0] == 'k') {
				std::cout << "Write yout message" << std::endl;
				std::cin >> Client_message.data();
				send(ClientSock, Client_message.data(), BUFF_SIZE, 0);

				recv(ClientSock, Server_message.data(), BUFF_SIZE, 0);
				std::cout << "Message sended" << std::endl;
				break;
			}
			else {
				std::cout << "Error" << std::endl;
				break;
			}
			break;
		case('3'):
			for (auto& p : user_arr) {
				if (name == p.first) {
					for (int i = 0; i < mes_arr.size(); i++) {
						std::cout << mes_arr[i] << std::endl;
						break;
					}
				}
			}
			break;
		case('4'):
			std::cout << "logout" << std::endl;
			b = false;
			break;
		default:
			std::cout << "error" << std::endl;
			break;
			}
		}
	}
}
