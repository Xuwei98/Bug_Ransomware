#pragma once
#ifndef RUNASADMIN_h
#define RUNASADMIN_h

#include"pch.h"

WORD GetSysVersion();
INT CallGetInformation(HANDLE hProcess);
DWORD VerifySecurityId(HANDLE hProcess);
VOID CallReleaseMutex();
LPCWSTR CallGetFilePath();
LPWSTR CallGetCommandLine();

#endif // !RUNASADMIN_h
