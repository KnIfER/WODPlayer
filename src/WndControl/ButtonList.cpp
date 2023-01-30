/** Copyright 2023 WODPLayer author
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
#include "ButtonList.h"
#include "InsituDebug.h"

#include "../resource.h"

extern VOID TOOLBAR_Register (void);

//ButtonList::ButtonList()
//{
//}

void ButtonList::init(HINSTANCE hInstance, HWND hParent)
{
	TOOLBAR_Register();
	WindowBase::init(hInstance, hParent);
	DWORD style = 
		WS_CHILD | WS_VISIBLE 
		| WS_CLIPCHILDREN | WS_CLIPSIBLINGS 
		| TBSTYLE_TOOLTIPS 
		| TBSTYLE_FLAT 
		//| CCS_NODIVIDER | CCS_NORESIZE | CCS_NOPARENTALIGN
		| CCS_BOTTOM
		| CCS_NODIVIDER
		;

	// WS_VISIBLE|WS_CHILD|TBSTYLE_FLAT|TBSTYLE_TOOLTIPS

	_hInst = hInstance;

	_hWnd = CreateWindowEx(0, L"MyToolbarWindow32", TEXT("toolbar"),
		style ,
		0, 0, 0, 0,
		hParent,
		(HMENU)0,
		hInstance,
		NULL);


	_hImgList = ImageList_Create(38, 38
		, ILC_COLOR32 | ILC_MASK, 3, 1);

	//ImageList_SetIconSize

	AddIcon(IDI_PLAY);
	AddIcon(IDI_STOP);
	AddIcon(IDI_PREV);
	AddIcon(IDI_NXT);
	AddIcon(IDI_FOLDER);


	SendWndMessage(TB_SETIMAGELIST, 0, (LPARAM)_hImgList);

	TBBUTTON buttonInfo[] =
	{
		{ 0, IDM_START,       TBSTATE_ENABLED
		, TBSTYLE_BUTTON|TBSTYLE_GROUP, {0}, 0, 0 }
		,{ 1, IDM_STOP,      TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_GROUP, {0}, 0, 0 }
		,{ 2, IDM_PREV,     TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_GROUP, {0}, 0, 0 }
		,{ 3, IDM_NXT,    TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_GROUP, {0}, 0, 0 }
		,{ 4, IDM_OPEN,    TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_GROUP, {0}, 0, 0 }
	};
	SendWndMessage(TB_ADDBUTTONS, 5, (LPARAM)&buttonInfo);

	SetWindowLongPtr(_hWnd, GWLP_USERDATA, (LONG_PTR)this);


	_SysWndProc = (WNDPROC)SetWindowLongPtr(_hWnd, GWLP_WNDPROC, (LONG_PTR)ButtonList::WndProc);


}

void ButtonList::AddIcon(int id)
{
	ImageList_AddIcon(_hImgList , LoadIcon(_hInst, MAKEINTRESOURCE(id)));
}

void ButtonList::ReplaceIcon(int pos, int id)
{
	ImageList_ReplaceIcon(_hImgList , pos, LoadIcon(_hInst, MAKEINTRESOURCE(id)));
}


// Adds one or more buttons to a ButtonList. pButtonInfoArray is a pointer to an array of TBBUTTON.
// Refer to TB_ADDBUTTONS in the Windows API documentation for more information.
BOOL ButtonList::AddButtons(UINT buttonCount, LPTBBUTTON pButtonInfoArray)
{
    return SendWndMessage(TB_ADDBUTTONS, (WPARAM)buttonCount, (LPARAM)pButtonInfoArray) != 0;
}


// Retrieves information about the specified button in a ButtonList.
// Refer to TB_GETBUTTON in the Windows API documentation for more information.
int ButtonList::GetCommandAt(int index)
{
	TBBUTTON tbb;
	ZeroMemory(&tbb, sizeof(tbb));
	SendWndMessage(TB_GETBUTTON, (WPARAM)index, (LPARAM)&tbb);

	// returns zero if failed
	return tbb.idCommand;
}



void ButtonList::showWindow()
{
	ShowWindow(_hWnd, SW_SHOW);
	UpdateWindow(_hWnd);
}


LRESULT ButtonList::RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	return ::CallWindowProc(_SysWndProc, hwnd, msg, w, l);
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