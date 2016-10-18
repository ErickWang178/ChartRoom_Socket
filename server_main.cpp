#include "server.h"
#include "msg.h"
#include <cstdlib>

#pragma comment(lib,"ws2_32.lib")


int main()
{
	WSADATA wsdData;
	//初始化WinSocket
	if(0 != WSAStartup(MAKEWORD(2,2),&wsdData))
	{
		Msg::exitPrompt("WinSocket初始化失败",WSAGetLastError());
		return 0;
	}

	Server *server = new Server();
	SOCKET serverSocket;
	in_addr serverIP = server->getHostIP();
	short serverPort = 1990;
	server->initServer(&serverSocket,&serverIP,serverPort);
	SOCKET clientSocket = server->recvConnect();
	server->run(clientSocket);


	WSACleanup();
	system("pause");
	return 0;
}