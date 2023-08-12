#include "header.h"

std::vector <char> Client_message(BUFF_SIZE), Server_message(BUFF_SIZE);

void Server_functions::message_distributor(SOCKET ClientConn) {
	while (true) {
		std::cout << "Waiting for the message" << std::endl;
		recv(ClientConn, Client_message.data(), BUFF_SIZE, 0);
		
		if (Client_message[0] == 'r') {
			Client_message.erase(Client_message.begin());
		}

		if (Client_message[0] == '1') {
			registration(ClientConn);
		}
		if (Client_message[0] == '2') {
			login(ClientConn);
		}
		if (Client_message[0] == '3') {
			message_all(ClientConn);
		}
		if (Client_message[0] == '4') {
			message_private(ClientConn);
		}
		if (Client_message[0] == '5') {
			show_messages(ClientConn);
		}
		if (Client_message[0] == '6') {
			logout(ClientConn);
		}
		if (Client_message[0] == '7') {
			quit(ClientConn);
			return;
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

	pstmt = con->prepareStatement("INSERT INTO user(username, password) VALUES(?,?)");

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
		if (Client_message[i] == ' ') break;
		mes += Client_message[i];
	}
	try {
		res = stmt->executeQuery("SELECT password FROM user WHERE username = '" + mes + "'");
	}
	catch (sql::SQLException e) {
		std::cout << e.what() << std::endl;
		Server_message[0] = 'r';
		send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
		return;
	}
	Server_message[0] = 'k';
	send(ClientConn, Server_message.data(), BUFF_SIZE, 0);

	recv(ClientConn, Client_message.data(), BUFF_SIZE, 0);
	if (Client_message[0] == 'r') return;

	try {
		res = stmt->executeQuery("SELECT password FROM user WHERE username = '" + mes + "'");
		while (res->next()) {
			std::string x = res->getString("password");
			if (Client_message.data() == x) {
				Server_message[0] = 'k';
				send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
				return;
			}
		}
	}
	catch (sql::SQLException e) {
		std::cout << e.what() << std::endl;
		Server_message[0] = 'r';
		send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
		return;
	}
}

void Server_functions::message_all(SOCKET ClientConn) {
	std::string sender_name, sender_id, mes;

	Client_message.erase(Client_message.begin());
	for (int i = 0; i < Client_message.size(); i++) {
		if (Client_message[i] == ' ') break;
		sender_name += Client_message[i];
	}
	try {
		res = stmt->executeQuery("SELECT id FROM user WHERE username = '" + sender_name + "'");
		while (res->next()) {
			sender_id = res->getString("id");
		}
		Server_message[0] = 'k';
		send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
		recv(ClientConn, Client_message.data(), BUFF_SIZE, 0);

		mes = sender_name + ": " + Client_message.data();
		std::cout << mes << std::endl;

		pstmt = con->prepareStatement("INSERT INTO messages(content, sender_id) VALUES(?,?)");
		pstmt->setString(1, mes);
		pstmt->setString(2, sender_id);
		pstmt->execute();
	}

	catch (sql::SQLException e) {
		std::cout << e.what() << std::endl;
		Server_message[0] = 'r';
		send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
		return;
	}

	send(ClientConn, Client_message.data(), BUFF_SIZE, 0);
	return;
}

void Server_functions::message_private(SOCKET ClientConn) {
	std::string recipient_name, sender_name, recipient_id, sender_id, mes;

	Client_message.erase(Client_message.begin());

	for (int i = 0; i < Client_message.size(); i++) {
		if (Client_message[i] == ' ') break;
		recipient_name += Client_message[i];
	}
	res = stmt->executeQuery("SELECT id FROM user WHERE username = '" + recipient_name + "'");
	while (res->next()) {
		recipient_id = res->getString("id");
	}

	Server_message[0] = 'k';
	send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
	recv(ClientConn, Client_message.data(), BUFF_SIZE, 0);

	for (int i = 0; i < Client_message.size(); i++) {
		if (Client_message[i] == ' ') break;
		sender_name += Client_message[i];
	}

	try {
		res = stmt->executeQuery("SELECT id FROM user WHERE username = '" + sender_name + "'");
		while (res->next()) {
			sender_id = res->getString("id");
		}

		send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
		recv(ClientConn, Client_message.data(), BUFF_SIZE, 0);

		mes = sender_name + " -> " + recipient_name + ": " + Client_message.data();
		std::cout << mes << std::endl;

		pstmt = con->prepareStatement("INSERT INTO messages(content, sender_id, recipient_id) VALUES(?,?,?)");
		pstmt->setString(1, mes);
		pstmt->setString(2, sender_id);
		pstmt->setString(3, recipient_id);
		pstmt->execute();
	}

	catch (sql::SQLException e) {
		std::cout << e.what() << std::endl;
		Server_message[0] = 'r';
		send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
		return;
	}

	send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
	return;
}

void Server_functions::show_messages(SOCKET ClientConn) {
	std::string recipient_name, recipient_id, mes;

	Client_message.erase(Client_message.begin());

	for (int i = 0; i < Client_message.size(); i++) {
		if (Client_message[i] == ' ') break;
		recipient_name += Client_message[i];
	}
	res = stmt->executeQuery("SELECT id FROM user WHERE username = '" + recipient_name + "'");
	while (res->next()) {
		recipient_id = res->getString("id");
	}

	try {
		res = stmt->executeQuery("SELECT content FROM messages WHERE recipient_id = '" + recipient_id + "'");
		while (res->next()) {
			mes += res->getString("content") + ", ";
		}
		for (int i = 0; i < mes.size(); i++) {
			Server_message[i] = mes[i];
		}

		send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
	}

	catch (sql::SQLException e) {
		std::cout << e.what() << std::endl;
		Server_message[0] = 'r';
		send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
		return;
	}
	return;
}

void Server_functions::logout(SOCKET ClientConn) {
	Client_message.erase(Client_message.begin());
	Server_message[0] = 'k';
	send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
	return;
}

void Server_functions::quit(SOCKET ClientConn) {
	Client_message.erase(Client_message.begin());
	Server_message[0] = 'k';
	send(ClientConn, Server_message.data(), BUFF_SIZE, 0);
	return;
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