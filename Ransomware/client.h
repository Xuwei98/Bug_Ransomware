#ifndef CLIENT_H
#define CLIENT_H

#include"pch.h"
#include"RansomFunction.h"

#pragma comment(lib, "ws2_32.lib")
bool SetTcpClient();
VOID	SendRequest();


#endif // !RansomFunction
