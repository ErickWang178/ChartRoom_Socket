#include "server.h"
#include "msg.h"
#include "helper.h"

#include<iomanip>

Server::Server(){
	cout << "***************Server已启动**************\n";
	cout << "..............等待客户端连接..........\n";
}

/************************************    
// 函数名称: getHostIP          
// 作 成 者：Erick.Wang     
// 作成日期：2016/10/17     
// 返 回 值: struct in_addr     
// 参    数: void  
// 函数说明：返回主机信息，包括主机名、IP地址等信息
           使用之间一定要使用WSAStartup函数初始化WinSocket，否则无法获取主机信息
//************************************/  
struct in_addr Server::getHostIP()
{
	char hostName[256];
	if (SOCKET_ERROR == gethostname(hostName,sizeof(hostName))){
		Msg::exitPrompt("获取主机名失败",WSAGetLastError());
	}

	struct hostent *hp = gethostbyname(hostName);
	if (hp == NULL){
		Msg::exitPrompt("获取主机IP地址失败",WSAGetLastError());
		exit(-1);
	}
	
	struct in_addr addr;
	memcpy(&addr,hp->h_addr_list[0],sizeof(struct in_addr));

	return addr;
}
/************************************    
// 函数名称: initServer          
// 作 成 者：Erick.Wang     
// 作成日期：2016/10/18     
// 返 回 值:      
// 参    数:   
// 函数说明：初始化服务器，设置服务器套接字，socketAddress
//************************************/  
void Server::initServer(SOCKET *serverSocket,struct in_addr *ip,short port){
	if (serverSocket == NULL || ip == NULL){
		Msg::exitPrompt("参数错误",WSAGetLastError());
		return ;
	}

	*serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (*serverSocket == SOCKET_ERROR){
		Msg::exitPrompt("创建套接字失败",WSAGetLastError());
		return ;
	}

	
	struct sockaddr_in serverAddress; 

	memset(&serverAddress,0,sizeof(struct sockaddr_in));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//进入绑定状态
	if(SOCKET_ERROR == bind(*serverSocket,(const sockaddr *)&serverAddress,sizeof(serverAddress))){
		Msg::exitPrompt("绑定到套接字失败",WSAGetLastError());
		return ;
	}
 
	//进入监听状态
	if (SOCKET_ERROR == listen(*serverSocket,SOMAXCONN)){
		Msg::exitPrompt("监听失败",WSAGetLastError());
		return ;
	}

	this->serverIP = getHostIP();
	this->serverSocket = *serverSocket;
	this->serverAddress = serverAddress;

	return ;
}


SOCKET Server::recvConnect(){
	 SOCKET clientSocket; //生成一个与客户端连接的套接字
	 struct sockaddr_in clientSocketAdderss;
	 int addrLen = sizeof(clientSocketAdderss);

	 while ((clientSocket = 
		   accept(serverSocket,(sockaddr *)&clientSocketAdderss,&addrLen)) == SOCKET_ERROR){
		 Msg::printPrompt("等待客户端连接......");
		 Sleep(1000);
		 
	 }
	 cout << "来自IP："<< inet_ntoa(clientSocketAdderss.sin_addr) <<" 连接成功.......\n";

	return clientSocket;
}

void Server::sendMsg(SOCKET &clientSocket){
	cout << "服务器说：";
	cin >> buf;
	if(send(clientSocket,buf,sizeof(buf),0) == SOCKET_ERROR){
		Msg::exitPrompt("Server发送数据失败",WSAGetLastError());
	}
}

void Server::recvMsg(SOCKET &clientSocket){
	int bytes;
	buf[0] = '\0'; 
	bytes = recv(clientSocket,buf,sizeof(buf),0);
	if(bytes == SOCKET_ERROR){
		buf[0] = '\0';
		Msg::exitPrompt("接收数据失败",WSAGetLastError());
	}else if (bytes == 0){
		cout << "客户端说已关闭\n";
	}else{
		cout << "客户端说："<< Helper::getDate();
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
