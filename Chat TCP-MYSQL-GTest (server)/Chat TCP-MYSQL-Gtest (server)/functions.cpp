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
		if (Client_message[0] == '2') {
			login(ClientConn);
		}
		else return;
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

void Server_functions::login(SOCKET ClientConn) {
	Client_message.erase(Client_message.begin());
	std::string mes;
	for (int i = 0; i < Client_message.size(); i++) {
		if (Client_message[i] == ' ') {
			Server_message[0] = 'r';
			send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
		}
		mes += Client_message[i];
	}
	try {
		res = stmt->executeQuery("SELECT password FROM user WHERE username = '" + mes + "'");
	}
	catch (sql::SQLException e) {
		std::cout << "There is no such user" << std::endl;
	}
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