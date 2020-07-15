// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。

extern "C" __declspec(dllexport) void Test_1(HWND hwnd, HINSTANCE hinst, LPTSTR lpCmdLine, INT nCmdShow) {

	MessageBox(NULL, TEXT("Test Yes 1"), TEXT("Yes"), MB_OK);

}
extern "C" __declspec(dllexport) void Test_2(HWND hwnd, HINSTANCE hinst, LPTSTR lpCmdLine, INT nCmdShow) {

	MessageBox(NULL, TEXT("Test Yes 2"), TEXT("Yes"), MB_OK);

}