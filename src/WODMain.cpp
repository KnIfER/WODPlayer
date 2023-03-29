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
int scheduleMoveWnd = 0;
extern RECT rcNScPos;


void hookMouseMove(MSG & msg)
{
	int yPos = msg.pt.y;
	RECT rc;
	GetWindowRect(XPP->GetHWND(), &rc);
	if(scheduleExitFsc)
	{
		//LogIs("hookMouseMove", msg.pt.x, msg.pt.y);
		if((GetKeyState(VK_LBUTTON) & 0x8000) != 0) // 从全屏窗口拖拽下标题栏
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
	if(XPP->_mainPlayer.IsMediaPlayerWindow(msg.hwnd)) 
	{
		//LogIs(2, "hookLButtonDown %d %d", msg.pt.x, msg.pt.y);
		if (!XPP->_isFullScreen && ::GetKeyState(VK_CONTROL) >= 0) // 拖拽窗体
		{
			scheduleMoveWnd = 1;
			//ReleaseCapture();
			//::SendMessage(XPP->GetHWND(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
			//SetFocus(XPP->GetHWND());
			//return;
		}
		if(XPP->_mainPlayer.GetHWND() != msg.hwnd && (XPP->_isFullScreen || ::GetKeyState(VK_CONTROL) < 0)) 
		{
			XPP->_mainPlayer.HandleCustomMessage(msg.message, msg.wParam, msg.lParam, 0);
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

extern void NavTimemark(int delta);

void hookMouseWheel(MSG & msg)
{
	//fwKeys = GET_KEYSTATE_WPARAM(wParam);
	auto zDelta = GET_WHEEL_DELTA_WPARAM(msg.wParam);
	if (msg.hwnd==XPP->_mainPlayer._seekbar.GetHWND() || IsKeyDown(VK_MENU))
	{
		NavTimemark(-zDelta);
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

std::vector<std::wstring> _args;

void parseCommandLine(const TCHAR* commandLine, std::vector<std::wstring>& arguments)
{
	int argc;
	LPWSTR* argv = CommandLineToArgvW(commandLine, &argc);
	for (int i = 0; i < argc; i++)
	{
		arguments.push_back(argv[i]);
	}
	LocalFree(argv);
}

int APIENTRY 
wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) 
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	parseCommandLine(lpCmdLine, _args);
	 
	// 初始化公共空间
	//INITCOMMONCONTROLSEX icc;
	//icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	//icc.dwICC = ICC_BAR_CLASSES;
	//InitCommonControlsEx(&icc);

	CPaintManagerUI::SetInstance(hInstance);
#ifdef _DEBUG
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("..//skin//"));
#else
	CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);
	//CPaintManagerUI::SetResourceZip(_T("skin.zip"));
	//CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
	HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), _T("IDR_ZIPRES"), _T("ZIPRES"));
	if( hResource != NULL ) {
		DWORD dwSize = 0;
		HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
		if( hGlobal != NULL ) {
			dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
			if( dwSize > 0 ) {
				CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);
			}
		}
		::FreeResource(hResource);
	}
#endif



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
	XPP->buildAccelerator();
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
				case WM_DROPFILES:
					XPP->HandleDropFiles(msg.message, msg.wParam, msg.lParam);
					break;
				case WM_LBUTTONDOWN:
					hookLButtonDown(msg);
					break;
				case WM_MOUSEWHEEL:
					hookMouseWheel(msg);
					break;
				case WM_MOUSEMOVE:
				case WM_NCMOUSEMOVE:
					if(scheduleMoveWnd) {
						if((GetKeyState(VK_LBUTTON) & 0x8000) != 0)
						{
							::SendMessage(XPP->GetHWND(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
							break;
						}
						scheduleMoveWnd = 0;
					}
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
				if (!TranslateAccelerator(XPP->GetHWND(), XPP->_hAccTable, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		//} catch(...) {
		//	LogIs(2, "Exception!");
		//}
	}
	//delete XPP;

	saveProf(usrDir, configFileName);
}



