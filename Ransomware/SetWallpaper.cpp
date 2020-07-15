#define _CRT_SECURE_NO_WARNINGS
#include"SetWallpaper.h"
VOID SaveBitmap(HDC hdc, HBITMAP hBmp,char* fileName)
{

	BITMAPINFO bmpInfo;
	BITMAPFILEHEADER  bmpFileHeader;
	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	GetDIBits(hdc, hBmp, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS);
	if (bmpInfo.bmiHeader.biSizeImage <= 0)
		bmpInfo.bmiHeader.biSizeImage = bmpInfo.bmiHeader.biWidth * abs(bmpInfo.bmiHeader.biHeight) * (bmpInfo.bmiHeader.biBitCount + 7) / 8;
	void* pBuf = malloc(bmpInfo.bmiHeader.biSizeImage);
	bmpInfo.bmiHeader.biCompression = BI_RGB;

	GetDIBits(hdc, hBmp, 0, bmpInfo.bmiHeader.biHeight, pBuf, &bmpInfo, DIB_RGB_COLORS);


	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpInfo.bmiHeader.biSizeImage;
	bmpFileHeader.bfType = 'MB';
	bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


	FILE* fp = NULL;
	fp = fopen(fileName, "wb");
	fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(pBuf, bmpInfo.bmiHeader.biSizeImage, 1, fp);
	fclose(fp);
}
BOOL SetWallpaper() {
	HDC hScrDC = GetDC(0);
	if (hScrDC) {
		HDC hMemDC = CreateCompatibleDC(hScrDC);
		if (hMemDC) {
			int xScrn, yScrn;
			xScrn = GetDeviceCaps(hScrDC, HORZRES);
			yScrn = GetDeviceCaps(hScrDC, VERTRES);
			HBITMAP hBitMap = CreateCompatibleBitmap(hScrDC, xScrn, yScrn);
			if (hBitMap) {
				SelectObject(hMemDC, hBitMap);
				int yPixel = GetDeviceCaps(hScrDC, VERTRES);
				int xPixel = GetDeviceCaps(hScrDC, HORZRES); 
				HFONT hFont = CreateFontW(70, 0, 4, 0, 0, 0, 0,0, ANSI_CHARSET, 0, 0, 0, 0, 0);
				if (hFont) {
					RECT rect = {0 ,0,xScrn,yScrn };
					SelectObject(hMemDC, hFont);
					SetBkMode(hMemDC, TRANSPARENT);
					SetTextColor(hMemDC, 0xFFFFFFFF);
					HGDIOBJ hBrush = GetStockObject(GRAY_BRUSH);
					FillRect(hMemDC, &rect, (HBRUSH)hBrush);
					for (int x = 1; x < xPixel; x++)
					{
						for (int y = 1; y < yPixel; y++)
						{
							SetPixel(hMemDC, x, y, RGB((y^x) % 255, (x * y) % 255, (x ^ y) % 255));
						}
					}
					string imageStr = base64_decode(Base64image);
					DrawTextExA( hMemDC,(LPSTR) imageStr.c_str(), imageStr.length(), &rect, DT_TOP,NULL);
					char *fileName = new char[MAX_PATH];
					const char* Dst = "C:\\Users\\";
					wstring& Sou = randstr(8);
					LPCSTR extension = ".bmp";
					sprintf(fileName, "%s%ws%s", Dst, Sou.c_str(),extension);
					SaveBitmap(hMemDC, hBitMap,fileName);
					if (!SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (PVOID)fileName, SPIF_SENDCHANGE | SPIF_UPDATEINIFILE)) {
						return FALSE;
					}
					return TRUE;
				}
			}
		}
	}
	return FALSE;
};
