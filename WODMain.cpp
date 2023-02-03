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
#include "pch.h"


struct DemoData
{
	const TCHAR* title;
	int image;
};

WODApplication* XPP;

void hookMouseMove(MSG & msg)
{
	int yPos = msg.pt.y;
	RECT rc;
	GetClientRect(XPP->GetHWND(), &rc);
	if (IsWindowVisible(XPP->_hFullScreenBtmbar))
	{
		if (!XPP->_mainPlayer._seekbar._isSeeking && yPos<rc.bottom-XPP->_barsHeight-12)
		{
			ShowWindow(XPP->_hFullScreenBtmbar, SW_HIDE);
		}
	}
	else if (yPos>=rc.bottom-XPP->_barsHeight) // -4
	{
		ShowWindow(XPP->_hFullScreenBtmbar, SW_SHOW);
	}
}

void hookLButtonDown(MSG & msg)
{
	if(XPP->IsFullScreen())
		return;
	LogIs("hookLButtonDown");
	if (XPP->_mainPlayer.IsMediaPlayerWindow(msg.hwnd)||msg.hwnd==XPP->GetHWND())
	{
		ReleaseCapture();
		::SendMessage(XPP->GetHWND(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		SetFocus(XPP->GetHWND());
		return;
	}
	if (msg.hwnd==XPP->_toolbar.GetHWND())
	{
		if (GET_X_LPARAM(msg.lParam)>XPP->_toolbar.GetHeight()*5)
		{
			ReleaseCapture();
			::SendMessage(XPP->GetHWND(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		}
		return;
	}
}

void hookLButtonDoubleClick(MSG & msg)
{
	//LogIs("hookLButtonDoubleClick");
	if (msg.hwnd==XPP->_toolbar.GetHWND())
	{
		if (GET_X_LPARAM(msg.lParam)>XPP->_toolbar.GetHeight()*5)
		{
			XPP->_mainPlayer.Toggle();
		}
		return;
	}
	if (XPP->_mainPlayer.IsMediaPlayerWindow(msg.hwnd))
	{
		XPP->_mainPlayer.Toggle();
		return;
	}
}

void hookMButtonClick(MSG & msg)
{
	if (msg.hwnd==XPP->_toolbar.GetHWND())
	{
		if (GET_X_LPARAM(msg.lParam)>XPP->_toolbar.GetHeight()*5)
		{
			XPP->ToggleFullScreen();
		}
		return;
	}
	if (XPP->_mainPlayer.IsMediaPlayerWindow(msg.hwnd))
	{
		XPP->ToggleFullScreen();
		SetFocus(XPP->GetHWND());
		return;
	}
}

bool running=true;

extern int testSqlite();

int APIENTRY 
wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) 
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//testSqlite();
	// 初始化公共空间
	//INITCOMMONCONTROLSEX icc;
	//icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	//icc.dwICC = ICC_BAR_CLASSES;
	//InitCommonControlsEx(&icc);

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("..//skin//"));

	XPP = new WODApplication();
	XPP->Create(NULL, _T("无限播放器"), UI_WNDSTYLE_FRAME, WS_EX_APPWINDOW|WS_EX_ACCEPTFILES, 0, 0, 500, 500);
	XPP->ShowWindow();
	XPP->CenterWindow();
	//XPP->init(hInstance, NULL);

	//XPP->ShowModal();

	MSG    msg;
	//while(running)
	{
		try {
			while (GetMessage(&msg, NULL, 0, 0))
			{
				switch (msg.message)
				{
				case WM_QUIT:
					running=false;
					break;
					break;
				case WM_LBUTTONDOWN:
					hookLButtonDown(msg);
					break;
				case WM_MOUSEMOVE:
					if (XPP->IsFullScreen())
						hookMouseMove(msg);
					break;
				case WM_MBUTTONUP:
					hookMButtonClick(msg);
					//case WM_RBUTTONDOWN:
					//	if (IsChild(XPP->GetMediaPlayerHWND(), msg.hwnd))
					//	{
					//		SetFocus(XPP->getHWND());
					//	}
					break;
				case WM_NCLBUTTONDBLCLK:
				case WM_LBUTTONDBLCLK:
					hookLButtonDoubleClick(msg);
					break;
				default:
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} catch(...) {
			LogIs(2, "Exception!");
		}
	}


	return msg.wParam;
}



