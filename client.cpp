#include "client.h"
#include "msg.h"
#include "helper.h"

Client::Client(){
	cout << "***************Client������**************\n";
	SOCKET s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (s == SOCKET_ERROR){
		Msg::exitPrompt("�����׽���ʧ��",WSAGetLastError());
	}
	 this->clientSocket = s;
}


void Client::getConnect(char *serverIP,short serverPort){
	if(serverIP == NULL) return;

	cout << "���ӷ�������" << serverIP << ":" << serverPort << endl;

	memset(&(this->serverIP),0,sizeof(struct in_addr));
	this->serverIP.S_un.S_addr = inet_addr(serverIP); //�����ʮ����IPתΪ����IP

	memset(&(serverSocketAddress),0,sizeof(struct sockaddr_in));
	serverSocketAddress.sin_family = AF_INET;
	serverSocketAddress.sin_port = htons(serverPort);
	serverSocketAddress.sin_addr.S_un.S_addr = inet_addr(serverIP);

	while(SOCKET_ERROR == connect(clientSocket,(sockaddr *)&serverSocketAddress,sizeof(struct sockaddr_in))){
		Msg::printPrompt("�����������������......");
		Sleep(1000);
	}
	Msg::printPrompt("����������ӳɹ�......");
	return ;
}

void Client::recvMsg(){
	//char buf[1024];
	int bytes;
 
	bytes = recv(clientSocket,buf,sizeof(buf),0);
	if (bytes == SOCKET_ERROR){
		Msg::exitPrompt("���ݽ��ܴ���",WSAGetLastError());
	}

	cout << "������˵��"<< Helper::getDate() << "    " << buf << endl;
 
}

void Client::sendMsg(){
	//char buf[1024];
 
	cout << "��˵��";
	cin >> buf;
	if (strlen(buf) <= 0 || buf == " "){
		Msg::printPrompt("�������ݲ���Ϊ��");
	}
	if (send(clientSocket,buf,sizeof(buf),0) == SOCKET_ERROR){
		Msg::exitPrompt("���ݷ���ʧ��",WSAGetLastError());
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