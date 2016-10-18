#include "client.h"
#include <cstdlib>

#pragma comment(lib,"ws2_32.lib")

int main()
{
	WSADATA wsaData;

	WSAStartup(MAKEWORD(2,2),&wsaData);

	Client *client = new Client();
	char *serverIP = "172.18.19.101";
	short serverPort = 1990;
	client->getConnect(serverIP,serverPort);
	client->run();

	WSACleanup();
	system("pause");
	return 0;
}