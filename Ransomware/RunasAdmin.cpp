#pragma once
#include"RunasAdmin.h"

static HANDLE hMutex;
WORD GetSysVersion() {
	WORD dwVersion;
	_asm {
		pushad
		mov ecx, fs:30h
		mov eax, [ecx + 0xa8]//次版本号
		mov ebx, [ecx + 0xa4]//主版本号
		shl ax, 8
		or ax, cx
		mov dwVersion, ax
		popad
	}
	return dwVersion;
};
INT CallGetInformation(HANDLE hProcess) {
	HANDLE hTokenHandle = NULL;
	INT dwNum = FALSE;
	if (OpenProcessToken(hProcess, TOKEN_QUERY, &hTokenHandle)) {
		DWORD dwSize = 0;
		BOOL bValue = GetTokenInformation(hTokenHandle,
			TokenElevationType,
			&dwNum,
			sizeof(DWORD),
			&dwSize); //提升权限
		dwNum = bValue != FALSE ? dwNum : FALSE;
		CloseHandle(hTokenHandle);
	}
	return dwNum;
};
DWORD VerifySecurityId(HANDLE hProcess) {
	DWORD result = 0;
	DWORD dwLengthNeeded;
	SID  Sid = {};
	HANDLE hTokenHandle;
	if (OpenProcessToken(hProcess, TOKEN_QUERY, &hTokenHandle)) {
		if (GetTokenInformation(hTokenHandle,
			TokenIntegrityLevel, //SID
			&Sid,
			sizeof(&Sid),
			&dwLengthNeeded)) {
			if (IsValidSid(&Sid)){
				// 管理员权限 SID S - 1 - 16 - 12288 12288 == 0x3000
				result = Sid.SubAuthority[4];
			}
		}
	}
	return result;
};
VOID CallReleaseMutex() {
	ReleaseMutex(hMutex);
};
LPCWSTR CallGetFilePath() {
	HMODULE hModule = GetModuleHandleW(NULL);
	static WCHAR path[MAX_PATH];
	GetModuleFileNameW(hModule, path, MAX_PATH);
	return path;
};
LPWSTR CallGetCommandLine() {
	int nArgs;
	LPWSTR cmdLine = GetCommandLineW();
	LPWSTR* szArglist = CommandLineToArgvW(cmdLine, &nArgs);
	if (nArgs <= 1)
		return NULL;
	else {
		return cmdLine;
	}
};
