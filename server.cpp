#include "server.h"
#include "msg.h"
#include "helper.h"

#include<iomanip>

Server::Server(){
	cout << "***************Server������**************\n";
	cout << "..............�ȴ��ͻ�������..........\n";
}

/************************************    
// ��������: getHostIP          
// �� �� �ߣ�Erick.Wang     
// �������ڣ�2016/10/17     
// �� �� ֵ: struct in_addr     
// ��    ��: void  
// ����˵��������������Ϣ��������������IP��ַ����Ϣ
           ʹ��֮��һ��Ҫʹ��WSAStartup������ʼ��WinSocket�������޷���ȡ������Ϣ
//************************************/  
struct in_addr Server::getHostIP()
{
	char hostName[256];
	if (SOCKET_ERROR == gethostname(hostName,sizeof(hostName))){
		Msg::exitPrompt("��ȡ������ʧ��",WSAGetLastError());
	}

	struct hostent *hp = gethostbyname(hostName);
	if (hp == NULL){
		Msg::exitPrompt("��ȡ����IP��ַʧ��",WSAGetLastError());
		exit(-1);
	}
	
	struct in_addr addr;
	memcpy(&addr,hp->h_addr_list[0],sizeof(struct in_addr));

	return addr;
}
/************************************    
// ��������: initServer          
// �� �� �ߣ�Erick.Wang     
// �������ڣ�2016/10/18     
// �� �� ֵ:      
// ��    ��:   
// ����˵������ʼ�������������÷������׽��֣�socketAddress
//************************************/  
void Server::initServer(SOCKET *serverSocket,struct in_addr *ip,short port){
	if (serverSocket == NULL || ip == NULL){
		Msg::exitPrompt("��������",WSAGetLastError());
		return ;
	}

	*serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (*serverSocket == SOCKET_ERROR){
		Msg::exitPrompt("�����׽���ʧ��",WSAGetLastError());
		return ;
	}

	
	struct sockaddr_in serverAddress; 

	memset(&serverAddress,0,sizeof(struct sockaddr_in));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//�����״̬
	if(SOCKET_ERROR == bind(*serverSocket,(const sockaddr *)&serverAddress,sizeof(serverAddress))){
		Msg::exitPrompt("�󶨵��׽���ʧ��",WSAGetLastError());
		return ;
	}
 
	//�������״̬
	if (SOCKET_ERROR == listen(*serverSocket,SOMAXCONN)){
		Msg::exitPrompt("����ʧ��",WSAGetLastError());
		return ;
	}

	this->serverIP = getHostIP();
	this->serverSocket = *serverSocket;
	this->serverAddress = serverAddress;

	return ;
}


SOCKET Server::recvConnect(){
	 SOCKET clientSocket; //����һ����ͻ������ӵ��׽���
	 struct sockaddr_in clientSocketAdderss;
	 int addrLen = sizeof(clientSocketAdderss);

	 while ((clientSocket = 
		   accept(serverSocket,(sockaddr *)&clientSocketAdderss,&addrLen)) == SOCKET_ERROR){
		 Msg::printPrompt("�ȴ��ͻ�������......");
		 Sleep(1000);
		 
	 }
	 cout << "����IP��"<< inet_ntoa(clientSocketAdderss.sin_addr) <<" ���ӳɹ�.......\n";

	return clientSocket;
}

void Server::sendMsg(SOCKET &clientSocket){
	cout << "������˵��";
	cin >> buf;
	if(send(clientSocket,buf,sizeof(buf),0) == SOCKET_ERROR){
		Msg::exitPrompt("Server��������ʧ��",WSAGetLastError());
	}
}

void Server::recvMsg(SOCKET &clientSocket){
	int bytes;
	buf[0] = '\0'; 
	bytes = recv(clientSocket,buf,sizeof(buf),0);
	if(bytes == SOCKET_ERROR){
		buf[0] = '\0';
		Msg::exitPrompt("��������ʧ��",WSAGetLastError());
	}else if (bytes == 0){
		cout << "�ͻ���˵�ѹر�\n";
	}else{
		cout << "�ͻ���˵��"<< Helper::getDate();
		cout <<  "    " << buf << endl;
	}
	
}

void Server::run(SOCKET &clientSocket){
	while (1){
		recvMsg(clientSocket);
		sendMsg(clientSocket);
	}
}

Server::~Server(){
	closesocket(this->serverSocket);
}
