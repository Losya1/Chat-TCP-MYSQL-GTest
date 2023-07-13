#include "header.h"

std::vector <char> Client_message(BUFF_SIZE), Server_message(BUFF_SIZE);

void Server_functions::message_distributor(SOCKET ClientConn) {
	while (true) {
		std::cout << "Waiting for the message" << std::endl;
		recv(ClientConn, Client_message.data(), BUFF_SIZE, 0);
		std::cout << Client_message.data() << std::endl;

		if (Client_message[0] == '1') {
			registration(ClientConn);
		}
	}
}

void Server_functions::registration(SOCKET ClientConn) {
	Client_message.erase(Client_message.begin());
	res = stmt->executeQuery("SELECT username FROM user");
	while (res->next()) {
		if (Client_message.data() == res->getString("username")) {
			Server_message[0] = 'r'; //r - error
			send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
			return;
		}
	}
	Server_message[0] = 'k'; //k - ok
	pstmt->setString(1, Client_message.data());

	send(ClientConn, Server_message.data(), BUFF_SIZE, 0);

	recv(ClientConn, Client_message.data(), BUFF_SIZE, 0);
	if (Client_message[0] == 'r') return;

	pstmt->setString(2, Client_message.data());
	pstmt->execute();

	Server_message[0] = 'k';
	send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
	std::cout << "One row inserted." << std::endl;
}

TEST(GTest, test1) {
	driver = get_driver_instance();
	con = driver->connect("tcp://127.0.0.1:3306", "root", "12345");
	con->setSchema("ChatDB");
	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT username FROM user");
	while (res->next()) {
		std::string x = res->getString("username");
		while (res->next()) {
			std::string y = res->getString("username");
			EXPECT_EQ(x, y);
		}
	}
}