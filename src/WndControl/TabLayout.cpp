/** Copyright 2021 KnIfER JK. Chen
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation.
*/
#include "TabLayout.h"

#include "resource.h"


extern void TAB_Register();

void TabLayout::init(HINSTANCE hInstance, HWND hParent)
{
	WindowBase::init(hInstance, hParent);
	TAB_Register();

	int style = WS_CHILD | WS_VISIBLE 
		| TCS_MULTILINE
		//| TCS_BUTTONS
		| TCS_FOCUSNEVER
		| WS_CLIPCHILDREN 

		| TCS_FLICKERFREE 

		;

	_hWnd = ::CreateWindowEx(
		0,
		L"MyTabControl32",
		TEXT("Tab"),
		style,
		0, 0, 800, 100,
		hParent,
		NULL,
		hInstance,
		0);

	//_hWnd = CreateWindow(L"MyTabControl32",      // window class name
	//	TEXT("App Tabs"),   // window caption
	//	WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX  
	//	| WS_MAXIMIZEBOX | WS_SIZEBOX
	//	| WS_CLIPCHILDREN 
	//	,
	//	CW_USEDEFAULT,// initial x position
	//	CW_USEDEFAULT,// initial y position
	//	CW_USEDEFAULT,// initial x size
	//	CW_USEDEFAULT,// initial y size
	//	NULL,                 // parent window handle
	//	NULL,            // window menu handle
	//	hInstance,   // program instance handle
	//	NULL);      // creation parameters

	SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	//_SysWndProc = (WNDPROC)GetWindowLongPtr(_hWnd, GWL_WNDPROC);

	_SysWndProc = (WNDPROC)SetWindowLongPtr(_hWnd, GWL_WNDPROC, (LONG_PTR)TabLayout::WndProc);


	HIMAGELIST hImageList = ImageList_Create(24, 24, ILC_COLOR24 | ILC_MASK, 3, 1);
	HBITMAP hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_TOOLBAR));
	ImageList_AddMasked(hImageList, hBitmap, RGB(255, 255, 255));
	DeleteObject(hBitmap);
	//SendMessage(hToolbar, TB_SETIMAGELIST, 0, (LPARAM)hImageList); // 正常显示时的图像列表

	TabCtrl_SetImageList(_hWnd, hImageList);

	//TabCtrl_SetMaxRows(_hWnd, 3);

	//TabCtrl_SetPadding(_hWnd, 12, 0);

}

void TabLayout::showWindow()
{
	ShowWindow(_hWnd, SW_SHOW);
	UpdateWindow(_hWnd);
}


void TabLayout::addTab(const TCHAR *text, int image)
{
	TCITEM data;
	data.iImage = image;
	data.mask = TCIF_TEXT;
	data.pszText = (TCHAR *)text;
	data.mask = TCIF_TEXT | TCIF_IMAGE;
	SendMessage(_hWnd, TCM_INSERTITEM, size++, reinterpret_cast<LPARAM>(&data));
}

LRESULT TabLayout::RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	if (_SysWndProc)
	{
		::CallWindowProc(_SysWndProc, hwnd, msg, w, l);
	}
	return ::DefWindowProc(hwnd, msg, w, l);
}

//static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
//{
//	GetWindowLongPtr(hwnd, GWLP_USERDATA);
//	static TCHAR s[] = TEXT("Hello, Windows.");
//	switch (msg)
//	{
//	case WM_DRAWITEM:
//	{
//		break;
//	}
//	case WM_CREATE:
//		return 0;
//	case WM_PAINT:
//		HDC hdc;
//		PAINTSTRUCT ps;
//		RECT rect;
//		GetClientRect(hwnd, &rect);
//		hdc = BeginPaint(hwnd, &ps);
//		DrawText (hdc, s, -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;
//		EndPaint(hwnd, &ps);
//		return 0;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//	case WM_SIZE:
//		if (_hWnd)
//		{
//			RECT rect;
//			GetClientRect(_hParent, &rect);
//			MoveWindow(_hWnd, rect.left, rect.top, rect.right, rect.bottom, true);
//		}
//		return 0;
//	case WM_QUIT:
//		return 0;
//	}
//}