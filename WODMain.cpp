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

int scheduleExitFsc = 0;
extern RECT rcNScPos;


void hookMouseMove(MSG & msg)
{
	int yPos = msg.pt.y;
	RECT rc;
	GetWindowRect(XPP->GetHWND(), &rc);
	if(scheduleExitFsc)
	{
		//LogIs("hookMouseMove", msg.pt.x, msg.pt.y);
		if((GetKeyState(VK_LBUTTON) & 0x8000) != 0)
		{
			if(abs(scheduleExitFsc-yPos)>5)
			{
				rcNScPos.bottom += yPos - rcNScPos.top;
				rcNScPos.top = yPos;

				int xPos = msg.pt.x, w = (rcNScPos.right-rcNScPos.left)/2;
				rcNScPos.left = xPos - w;
				rcNScPos.right = xPos + w;

				scheduleExitFsc = 0;
				XPP->ToggleFullScreen();
				ReleaseCapture();
				::SendMessage(XPP->GetHWND(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
			}
			return;
		}
		else scheduleExitFsc = 0;
	}
	if(XPP->_bottomBar->IsVisible() ^ (yPos >= rc.bottom - XPP->_bottomBar->GetHeight()))
	{
		XPP->_bottomBar->SetVisible(!XPP->_bottomBar->IsVisible());
	}
	if(XPP->_topBarFscWnd->IsVisible() ^ (yPos <= rc.top + XPP->_topBar->GetHeight()))
	{
		XPP->_topBarFscWnd->SetVisible(!XPP->_topBarFscWnd->IsVisible());
	}
}

void hookLButtonDown(MSG & msg)
{
	//LogIs("hookLButtonDown %d %d", msg.pt.x, msg.pt.y);
	if (!XPP->_isFullScreen)
	{
		if(XPP->_mainPlayer.IsMediaPlayerWindow(msg.hwnd)) 
		{
			ReleaseCapture();
			::SendMessage(XPP->GetHWND(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
			//SetFocus(XPP->GetHWND());
			return;
		}
	}
	else if(msg.pt.y<=4 || XPP->_topBarFscWnd->GetHWND()==msg.hwnd) 
	{
		scheduleExitFsc = msg.pt.y;
		if(scheduleExitFsc==0)
			scheduleExitFsc=1;
		return;
	}
}

void hookLButtonDoubleClick(MSG & msg)
{
	//LogIs("hookLButtonDoubleClick");
	//if (msg.hwnd==XPP->_toolbar.GetHWND())
	//{
	//	if (GET_X_LPARAM(msg.lParam)>XPP->_toolbar.GetHeight()*5)
	//	{
	//		XPP->_mainPlayer.Toggle();
	//	}
	//	return;
	//}
	if (XPP->_mainPlayer.IsMediaPlayerWindow(msg.hwnd))
	{
		XPP->_mainPlayer.Toggle();
		return;
	}
}

void hookMButtonClick(MSG & msg)
{
	//if (msg.hwnd==XPP->_toolbar.GetHWND())
	//{
	//	if (GET_X_LPARAM(msg.lParam)>XPP->_toolbar.GetHeight()*5)
	//	{
	//		XPP->ToggleFullScreen();
	//	}
	//	return;
	//}
	if (XPP->_mainPlayer.IsMediaPlayerWindow(msg.hwnd))
	{
		XPP->ToggleFullScreen();
		SetFocus(XPP->GetHWND());
		return;
	}
}

bool running=true;

TCHAR usrDir[MAX_PATH];
const TCHAR* configFileName = L"wod.ini";

int APIENTRY 
wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) 
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 初始化公共空间
	//INITCOMMONCONTROLSEX icc;
	//icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	//icc.dwICC = ICC_BAR_CLASSES;
	//InitCommonControlsEx(&icc);

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("..//skin//"));

	TCHAR usrDir[MAX_PATH];
	::GetModuleFileName(NULL, usrDir, MAX_PATH);
	::PathRemoveFileSpec(usrDir);
	//::PathAppend();
	loadProf(usrDir, configFileName);

	XPP = new WODApplication();
	//WODApplication app{};
	//XPP = new WODApplication();
	//XPP = &app;
	XPP->Create(NULL, _T("无限播放器"), UI_WNDSTYLE_FRAME, WS_EX_APPWINDOW|WS_EX_ACCEPTFILES, 0, 0, 500, 500);
	XPP->ShowWindow();
	XPP->CenterWindow();
	//XPP->init(hInstance, NULL);

	//XPP->ShowModal();
	CControlUI* btn = new CControlUI();

	MSG    msg;
	//while(running)
	{
		//try {
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
				case WM_NCMOUSEMOVE:
					if (XPP->_isFullScreen)
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
		//} catch(...) {
		//	LogIs(2, "Exception!");
		//}
	}
	//delete XPP;

	saveProf(usrDir, configFileName);
}



