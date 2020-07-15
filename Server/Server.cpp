// Server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<WinSock2.h>

using namespace std;
#pragma comment(lib, "wsock32.lib")
bool SetTcpServer()
{
	//1.加载套接字库
	WORD w_version_req = MAKEWORD(2, 2); //初始化WinSock版本号
	WSADATA wsaData;
	int flag_InitWSA = WSAStartup(w_version_req, &wsaData); //具体参数含义参考：https://blog.csdn.net/clemontine/article/details/53141041
	//flag_InitWSA 不为0则说明初始化失败
	if (flag_InitWSA != 0)
	{
		cout << "初始化WSAStartup失败！" << endl;
		return false;
	}
	//wsaData为空指针，说明初始化失败
	if (&wsaData == nullptr)
	{
		cout << "初始化失败,&wsaData为空指针！" << endl;
		return false;
	}

	//2.创建套接字
	SOCKET socketServer = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN socketAddr;
	socketAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //需要接收任意的IP地址
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(12306);//端口号

	//3.连接套接字
	bind(socketServer, (SOCKADDR*)&socketAddr, sizeof(SOCKADDR));

	//4.设置监听客户端数量
	listen(socketServer, 5);
	cout << "启动监听" << endl;

	SOCKADDR_IN sockclient;
	int len = sizeof(SOCKADDR);
	SOCKET sockCon = accept(socketServer, (SOCKADDR*)&sockclient, &len);//创建链接套接字

	//5.发送数据到客户端
	const char* sendBuf = "Server";
	send(sockCon, sendBuf, strlen(sendBuf) + 1, 0);

	//6.接收客户端发来的消息
	char recvBuf[250];
	recv(sockCon, recvBuf, 250, 0);//发送\接收函数
	cout << "收到的客户端发来的信息为：" << recvBuf << endl;



	char recvBuf_2[250];
	recv(sockCon, recvBuf_2, 250, 0);//发送\接收函数
	cout << "收到的客户端发来的信息为：" << recvBuf_2 << endl;

	//7.关闭服务器
	closesocket(socketServer);
	closesocket(sockCon);//关闭套接字
	WSACleanup();
	return true;
}

int main(int argc, TCHAR* argv[])
{
	while (1) {
		SetTcpServer();
		cout << "服务器关闭！" << endl;
		getchar();
	}
	return 0;
}


