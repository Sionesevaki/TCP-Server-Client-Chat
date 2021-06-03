#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <iostream>
#include <arpa/inet.h>

#define PORT 54000

int main() {

	int sock;
	int valread;
	struct sockaddr_in serv_address;
	const char *cli_quit = "Client has ended the session";
	char buffer[1024] = {0};

	//************ create socket *************

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		std::cout << "Error creating socket" << std::endl;
		return 0;
	}

	//set up addr/port of server
	serv_address.sin_family = AF_INET;
	serv_address.sin_port = htons(PORT);

	//convert IPv4 and IPv6  addresses from text to binary form
	int _bin = inet_pton(AF_INET, "127.0.0.1", &serv_address.sin_addr);
	if(_bin <= 0) {
		std::cout << "ERROR: Address not supported" << std::endl;
		return 0;
	}

	//**************** create connection **************
	int _conn = connect(sock, (struct sockaddr*)&serv_address, sizeof(serv_address));
	if(_conn < 0) {
		std::cout << "ERROR: connection unsuccessful" << std::endl;
		return 0;
	}
	std::cout << "Connected to Server! Type message: " << std::endl;

	while(true) {
		
		memset(&buffer, 0, sizeof(buffer));
		std::string msg;
		std::cout << "> ";

		getline(std::cin, msg);
		
		strcpy(buffer, msg.c_str());
		
		if(!strcmp((char*)&msg, "exit")) {
			send(sock, cli_quit, strlen(cli_quit), 0);
			//std::cout << "Server has ended session" << std::endl;
			break;
		}
		send(sock, (char*)&buffer, strlen(buffer), 0);


		memset(&buffer, 0, sizeof(buffer));
		valread = read(sock, buffer, 1024);
		std::cout << "SERVER: " <<  buffer << std::endl;
		

	}

	//close socket

	close(sock);
	std::cout << "CONNECTION HAS ENDED WITH SERVER" << std::endl;

	/*

	send(sock, hello, strlen(hello), 0);
	std::cout << "Hello message sent to server!" << std::endl;

	valread = read(sock, buffer, 1024);
	std::cout << buffer << std::endl;
*/


	return 0;


}