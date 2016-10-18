#ifndef SEERVER_H
#define SEERVER_H

#include <iostream>
#include <WinSock.h>
using namespace std;

class Server{
private:
	SOCKET serverSocket;
	struct in_addr serverIP;
	sockaddr_in serverAddress;
	static const unsigned int maxsize = 1024;
	char buf[maxsize];
	

public:
	Server();
	struct in_addr getHostIP();

	void initServer(SOCKET *s,struct in_addr *ip,short port); 
	SOCKET recvConnect();
	void recvMsg(SOCKET &clientSocket); 
	void sendMsg(SOCKET &clientSocket); 
	void run(SOCKET &clientSocket);

	~Server();

};


#endif