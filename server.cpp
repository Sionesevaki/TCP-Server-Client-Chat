#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <iostream>

#define PORT 54000

int main() {

	int sockfd; //socket file descriptor
	int newsocket; //for established connection
	int valread;
	int opt = 1;
	struct sockaddr_in address; //addr/port
	char buffer[1024] = {0}; //for messages
	const char *serv_quit = {"Server has end session!"};

	//****CREATE A SOCKET FILE DESCRIPTOR********
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

		//check for error
	if(sockfd < 0) {
		std::cout << "ERROR: Socket cannot be established" << std::endl;
		return 0;
	}

	//*******SETSOCKOPT: ************
	// helps prevent errors like address already in use
	// reuse of the address and port
	// setsockopt(socket descriptor, socket name, options, ptr to opt val, size of opt)
	/*int _sockopt = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	if(_sockopt < 0) {
		std::cout << "ERROR: sockopt fail" << std::endl;
		return 0;
	}*/
		//setup address and port
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY; //localhost
	address.sin_port = htons(PORT);


	//******BIND IP/PORT TO SOCKET*******
	// cast adrress ptr to sockaddr: this is safe to do
	// bind(struct sockaddr ptr, size of sockaddr_in)
	int _bnd = bind(sockfd, (struct sockaddr *)&address, sizeof(address));

	if(_bnd < 0) {
		std::cout << "ERROR: could not bind socket" << std::endl;
		return 0;
	}

	//******* SET SOCKET TO PASSIVE MODE/ LISTEN *******
	int _socklisten = listen(sockfd, 5);

	if(_socklisten < 0) {
		std::cout << "ERROR: listening unsuccessful" << std::endl;
		return 0;
	}

	//********* ACCEPT COONECION: Establish new socket ********

	newsocket = accept(sockfd, (struct sockaddr *)&address, (socklen_t*)&address);

	if(newsocket < 0) {
		std::cout << "ERROR: cannot accept connection" << std::endl;
		return 0;
	}
	std::cout << "Connected with client! " << std::endl;

	//communicate with client
	while(true) {
		
		memset(&buffer, 0, sizeof(buffer)); // clear the buffer

		//read message from client
		valread = read(newsocket, buffer, 1024);

		if(!strcmp(buffer, "exit") ) {
			std::cout << "Client quit session" << std::endl;
			break;
		}

		std::cout << "CLIENT: " << buffer << std::endl;
		std::cout << "> " ;

		std::string data;
		getline(std::cin, data);

		memset(&buffer, 0, sizeof(buffer));
		strcpy(buffer, data.c_str());

		if(data == "exit") {
			send(newsocket, serv_quit, strlen(serv_quit), 0);
			break;
		}

		//send message to client
		send(newsocket, (char*)&buffer, strlen(buffer), 0);

	}

	//close socket
	close(newsocket);
	close(sockfd);
	std::cout << "CONNECTION HAS ENDED WITH CLIENT" << std::endl;
 
	/*

		//read message from client
	valread = read(newsocket, buffer, 1024);
	std::cout << "%s\n" << buffer << std::endl;

		//send message to client
	send(newsocket, hello, strlen(hello), 0);
	std::cout << "Message Sent!" << std::endl;
*/

	return 0;
}