#include "WindowBase.h"
#include "gdiplus.h"

LONG_PTR WOD_IMG_UTILS(const char* msg, HWND hWnd, LONG_PTR wParam, LONG_PTR lParam) {
	if(!strcmp(msg, "screenshot")) { // https://stackoverflow.com/questions/3291167/how-can-i-take-a-screenshot-in-a-windows-application
		HDC hScreenDC = GetDC(nullptr); // CreateDC("DISPLAY",nullptr,nullptr,nullptr);
		HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
		int width = GetDeviceCaps(hScreenDC,HORZRES);
		int height = GetDeviceCaps(hScreenDC,VERTRES);
		HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC,width,height);
		HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC,hBitmap));
		BitBlt(hMemoryDC,0,0,width,height,hScreenDC,0,0,SRCCOPY);
		hBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC,hOldBitmap));


		OpenClipboard(NULL);
		EmptyClipboard();
		SetClipboardData(CF_BITMAP, hBitmap);
		CloseClipboard();


		DeleteDC(hMemoryDC);
		DeleteDC(hScreenDC);
	}
	if(!strcmp(msg, "screenshotie")) {
		RECT rc;
		GetWindowRect(hWnd, &rc);

		HDC hScreenDC = GetDC(nullptr);
		HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;
		HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC,width,height);
		HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC,hBitmap));
		BitBlt(hMemoryDC,0,0,width,height,hScreenDC,rc.left,rc.top,SRCCOPY);
		hBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC,hOldBitmap));


		OpenClipboard(NULL);
		EmptyClipboard();
		SetClipboardData(CF_BITMAP, hBitmap);
		CloseClipboard();


		DeleteDC(hMemoryDC);
		DeleteDC(hScreenDC);
	}
	if(!strcmp(msg, "capture_window")) { // https://stackoverflow.com/questions/7292757/how-to-get-screenshot-of-a-window-as-bitmap-object-in-c
		//initialize Gdiplus once:
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		RECT rc;
		GetClientRect(hWnd, &rc);

		//create
		HDC hdcScreen = GetDC(NULL);
		HDC hdc = CreateCompatibleDC(hdcScreen);
		HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, 
			rc.right - rc.left, rc.bottom - rc.top);
		SelectObject(hdc, hbmp);

		//Print to memory hdc
		PrintWindow(hWnd, hdc, PW_CLIENTONLY);

		//copy to clipboard
		OpenClipboard(NULL);
		EmptyClipboard();
		SetClipboardData(CF_BITMAP, hbmp);
		CloseClipboard();

		//release
		DeleteDC(hdc);
		DeleteObject(hbmp);
		ReleaseDC(NULL, hdcScreen);
		return 0;
	}
	if(!strcmp(msg, "")) {
		return 0;
	}
	return -1;
}


