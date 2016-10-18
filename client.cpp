#include "client.h"
#include "msg.h"
#include "helper.h"

Client::Client(){
	cout << "***************Client已启动**************\n";
	SOCKET s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (s == SOCKET_ERROR){
		Msg::exitPrompt("创建套接字失败",WSAGetLastError());
	}
	 this->clientSocket = s;  
}


void Client::getConnect(char *serverIP,short serverPort){
	if(serverIP == NULL) return;

	cout << "连接服务器：" << serverIP << ":" << serverPort << endl;

	memset(&(this->serverIP),0,sizeof(struct in_addr));
	this->serverIP.S_un.S_addr = inet_addr(serverIP); //将点分十进制IP转为网络IP

	memset(&(serverSocketAddress),0,sizeof(struct sockaddr_in));
	serverSocketAddress.sin_family = AF_INET;
	serverSocketAddress.sin_port = htons(serverPort);
	serverSocketAddress.sin_addr.S_un.S_addr = inet_addr(serverIP);

	while(SOCKET_ERROR == connect(clientSocket,(sockaddr *)&serverSocketAddress,sizeof(struct sockaddr_in))){
		Msg::printPrompt("与服务器建立连接中......");
		Sleep(1000);
	}
	Msg::printPrompt("与服务器连接成功......");
	return ;
}

void Client::recvMsg(){
	//char buf[1024];
	int bytes;
 
	bytes = recv(clientSocket,buf,sizeof(buf),0);
	if (bytes == SOCKET_ERROR){
		Msg::exitPrompt("数据接受错误",WSAGetLastError());
	}

	cout << "服务器说："<< Helper::getDate() << "    " << buf << endl;
 
}

void Client::sendMsg(){
	//char buf[1024];
 
	cout << "我说：";
	cin >> buf;
	if (strlen(buf) <= 0 || buf == " "){
		Msg::printPrompt("发送数据不能为空");
	}
	if (send(clientSocket,buf,sizeof(buf),0) == SOCKET_ERROR){
		Msg::exitPrompt("数据发送失败",WSAGetLastError());
	}
	 
}

void Client::run(){
	while (1){
		sendMsg();
		recvMsg();
	}
}

Client::~Client(){
	closesocket(clientSocket);
}
