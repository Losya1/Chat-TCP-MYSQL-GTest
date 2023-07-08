#include "header.h"

void User::registration(SOCKET ClientConn) {
	Client_message.erase(Client_message.begin());
	std::cout << Client_message.data() << std::endl;

	res = stmt->executeQuery("SELECT username FROM user");
	while (res->next()) {
		Server_message.clear();
		Server_message.push_back('107');

		if (Client_message.data() == res->getString("username")) {
			return;
		}
	}
	pstmt->setString(1, Client_message.data());

	send(ClientConn, Server_message.data(), BUFF_SIZE, 0);

	recv(ClientConn, Client_message.data(), BUFF_SIZE, 0);
	pstmt->setString(2, Client_message.data());
	pstmt->execute();

	send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
	std::cout << "One row inserted." << std::endl;
}