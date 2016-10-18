#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <WinSock.h>

using namespace std;

class Client{
private:
	SOCKET clientSocket;
	struct in_addr serverIP;
	struct sockaddr_in serverSocketAddress;
	static const unsigned int maxsize = 1024;
	char buf[maxsize];
	
public:
	Client();

	void getConnect(char *serverIP,short serverPort);
	void sendMsg();
	void recvMsg();

	void run();

	~Client();

};

#endif