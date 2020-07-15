#ifndef CRYPTFUNCTION_H
#define CRYPTFUNCTION_H



#include"pch.h"
#include "iocp\CopyFiles.h"
#include"IOCP/IoPool.h"
#include"RansomFunction.h"


BOOL WriteReadme(wstring lpAddress);

//bool ListFiles(wstring path, wstring mask, vector<wstring>& files);
VOID CallListfile();
VOID CryptMainFunction();
#endif // !RansomFunction
