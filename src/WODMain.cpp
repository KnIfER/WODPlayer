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

extern void makeTopmost(HWND hwnd, bool mayTop);

CContainerUI* toHide;

BOOL iconized = FALSE;

void hookMouseMove(MSG & msg)
{
	if(iconized) return;
	int yPos = msg.pt.y;
	RECT rc;
	GetWindowRect(XPP->GetHWND(), &rc);
	if(scheduleExitFsc)
	{
		//LogIs("hookMouseMove", msg.pt.x, msg.pt.y);
		if((!_trackingMenu && GetKeyState(VK_LBUTTON) & 0x8000) != 0) // 从全屏窗口拖拽下标题栏
		{
			if(abs(scheduleExitFsc-yPos)>5)
			{
				if(XPP->_isFullScreen) {
					rcNScPos.bottom += yPos - rcNScPos.top;
					rcNScPos.top = yPos;

					int xPos = msg.pt.x, w = (rcNScPos.right-rcNScPos.left)/2;
					rcNScPos.left = xPos - w;
					rcNScPos.right = xPos + w;

					scheduleExitFsc = 0;
					XPP->ToggleFullScreen();
					ReleaseCapture();
					XPP->ToggleFullScreen1();
				}
				::SendMessage(XPP->GetHWND(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
			}
			return;
		}
		else scheduleExitFsc = 0;
	}
	bool v = !XPP->_bottomBar->IsVisible();
	if((!v) ^ (yPos >= rc.bottom - XPP->_bottomBar->GetHeight()))
	{
		if(!v && _menuShown) toHide = (XPP->_bottomBar);
		else XPP->_bottomBar->SetVisible(v);
		v = !v;
	} 
	if(!v && XPP->_hFscBtmbar) {
		makeTopmost(XPP->_bottomBar->GetHWND(), 1);
	}

	v = !XPP->_topBarFscWnd->IsVisible();
	if((!v) ^ (yPos <= rc.top + XPP->_topBar->GetHeight()))
	{
		if(!v && _menuShown) toHide = (XPP->_topBarFscWnd);
		else XPP->_topBarFscWnd->SetVisible(v);
		v = !v;
	}
	if(!v && XPP->_hFscBtmbar) {
		makeTopmost(XPP->_topBarFscWnd->GetHWND(), 1);
	}
}

void hookLButtonDown(MSG & msg)
{
	//LogIs("hookLButtonDown %d %d", msg.pt.x, msg.pt.y);
	if(XPP->_mainPlayer.IsMediaPlayerWindow(msg.hwnd)) 
	{
		//LogIs(2, "hookLButtonDown %d %d", msg.pt.x, msg.pt.y);
		BOOL fullscreen = XPP->_isFullScreen || XPP->_isMini;
		if (!XPP->_isFullScreen && ::GetKeyState(VK_CONTROL) >= 0) // 拖拽窗体
		{
			scheduleMoveWnd = 1;
			//ReleaseCapture();
			//::SendMessage(XPP->GetHWND(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
			//SetFocus(XPP->GetHWND());
			//return;
		}
		if(XPP->_mainPlayer.GetHWND() != msg.hwnd && (fullscreen || ::GetKeyState(VK_CONTROL) < 0)) 
		{
			XPP->_mainPlayer.HandleCustomMessage(msg.message, msg.wParam, msg.lParam, 0);
		}
	}
	//else if(XPP->_topBarFscWnd->GetHWND()==msg.hwnd) 
	//{
	//	//scheduleMoveWnd = 1;
	//	::SendMessage(XPP->GetHWND(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
	//	SetFocus(XPP->GetHWND());
	//}
	else if(msg.pt.y<=4 || XPP->_topBarFscWnd->GetHWND()==msg.hwnd) 
	{
		scheduleExitFsc = msg.pt.y;
		if(scheduleExitFsc==0)
			scheduleExitFsc=1;
	}
}

extern void NavTimemark(int delta);

void hookMouseWheel(MSG & msg)
{
	//fwKeys = GET_KEYSTATE_WPARAM(wParam);
	auto zDelta = GET_WHEEL_DELTA_WPARAM(msg.wParam);
	if(false)
	if (msg.hwnd==XPP->_mainPlayer._seekbar.GetHWND() || IsKeyDown(VK_MENU))
	{
		NavTimemark(-zDelta);
	}
	//else if (XPP->_mainPlayer.IsMediaPlayerWindow(msg.hwnd))
	//{
	//	TEventUI event = { 0 };
	//	event.Type = UIEVENT_SCROLLWHEEL;
	//	event.pSender = &XPP->_mainPlayer;
	//	//event.wParam = MAKEWPARAM(zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, zDelta);
	//	event.wParam = msg.wParam;
	//	event.lParam = msg.lParam;
	//	XPP->_mainPlayer.DoEvent(event);
	//}

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
		XPP->ToggleFullScreen1();
		return;
	}
}	

bool running=true;

TCHAR usrDir[MAX_PATH];
const TCHAR* configFileName = L"wod.ini";

bool keyPressed = 0;

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
#include <codecvt>

BOOL prvInstance(_In_ LPWSTR lpCmdLine, BOOL add)
{
	if (!IsKeyDown(VK_CONTROL))
	{
		// 已有实例在运行，将参数传递给已有实例
		COPYDATASTRUCT cds;
		cds.dwData = WOD_COPYDATA;
		cds.cbData = 0;
		QkString tmp;
		if (_args.size()) {
			if(add) {
				tmp = lpCmdLine;
				tmp.Append(L" -add   ");
				cds.cbData = (tmp.GetLength()) * sizeof(WCHAR);
				cds.lpData = lpCmdLine = (LPWSTR)STR(tmp);
				lpCmdLine[tmp.GetLength()-1] = L'\0';
			} else {
				cds.cbData = (lstrlen(lpCmdLine)) * sizeof(WCHAR);
				cds.lpData = lpCmdLine;
				lpCmdLine[lstrlen(lpCmdLine)] = L'\0';
			}
		}
		if(!add) 
		{
			HWND hWnd = FindWindow(L"WODPlayer", readyT);
			if (hWnd != NULL)
			{
				SendMessage(hWnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds);
				return TRUE; // 退出当前实例
			}
			return FALSE;
		}
		HWND hWnd = FindWindow(L"WODPlayer", NULL);
		if (hWnd != NULL)
		{
			SendMessage(hWnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds);
		}
		return TRUE; // 退出当前实例
	}
	return FALSE;
}

TCHAR* ReadUTF16File(const TCHAR* filePath) {
	HANDLE hFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		// Handle file opening error
		return NULL;
	}

	DWORD fileSize = GetFileSize(hFile, NULL);
	TCHAR* buffer = new TCHAR[fileSize / sizeof(TCHAR) + 1];
	DWORD bytesRead = 0;
	ReadFile(hFile, buffer, fileSize, &bytesRead, NULL);
	buffer[fileSize / sizeof(TCHAR)] = '\0';

	CloseHandle(hFile); 

	return buffer;
}


int initX=-1, initY=-1, initW=500, initH=500, initM;


void parseArgs(std::vector<std::wstring> & args) 
{
	bool append = std::find(args.begin(), args.end(), L"-add")!= args.end();
	for (size_t i = 0; i < args.size(); i++)
	{
		auto &  path = args[i];
		//lxx(ss, (path.data()))
		if(path.size()>0) {
			if(path[0]==L'-') {
				auto data = args[i].c_str();
				//lxx(ss, data)
				if(StrCmpN(data, L"-loadArgs", 9)==0) {
					QkString ln;
					if(StrCmpN(data, L"-loadArgsW", 10)==0) {
						TCHAR* all = ReadUTF16File(data+11)+1;
						TCHAR* current = all;
						TCHAR* next = nullptr;
						while ((next = _tcschr(current, _T('\n'))) != nullptr) {
							// Process the line from current to next
							ln.Empty();
							ln.Append(current, next-current);
							//*next = _T('\0');
							//_tprintf(_T("%s\n"), current);
							*next = _T('\n'); // Restore the newline character
							current = next + 1; // Move to the character after the newline
							ln.Trim();
							args.push_back(ln.GetData());
						}
						if (*current != _T('\0')) {
							//_tprintf(_T("%s\n"), current);
							ln = current;
							ln.Trim();
							args.push_back(ln.GetData());
						}


						//	std::wifstream file(data+11, std::ios::binary);
						//	if (file.is_open()) {
						//		// apply BOM-sensitive UTF-16 facet
						//		std::locale loc("chs");	
						//		std::wcout.imbue(loc);
						//		std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>* codecvtToUnicode = new std::codecvt_utf16 < wchar_t, 0x10ffff, std::consume_header > ;
						//		file.imbue(std::locale(file.getloc(), codecvtToUnicode));
						//		std::wstring wline;
						//		while (std::getline(file, wline)) {
						//			ln = wline.data();
						//			ln.Trim();
						//			////lxx(ss dd, ln.GetData(), ln==L"J:\\R\\-- Foliage --\\life\\睡不着的泡泡糖： -  #甜妹统治世界.mp4")

						//			//if(ln.GetLength())
						//			args.push_back(ln.GetData());
						//			//args.push_back(wline);
						//		}
						//	}
						//	file.close();
					} 
					else {
						std::ifstream file(data+10);
						if (file.is_open()) {
							std::string line;
							while (std::getline(file, line)) {
								ln = line.c_str();
								//lxx(ss, ln.GetData())
								args.push_back(STR(ln));
							}
						}
						file.close();
					}
				}

				else if(StrCmpN(data, L"-init", 5)==0) {
					long tmp;
					if(StrCmpN(data, L"-initX", 6)==0) {
						STR2Decimal(data+7, tmp);
						initX = tmp;
					}
					if(StrCmpN(data, L"-initY", 6)==0) {
						STR2Decimal(data+7, tmp);
						initY = tmp;
					}
					if(StrCmpN(data, L"-initW", 6)==0) {
						STR2Decimal(data+7, tmp);
						initW = tmp;
					}
					if(StrCmpN(data, L"-initH", 6)==0) {
						STR2Decimal(data+7, tmp);
						initH = tmp;
					}
					if(StrCmpN(data, L"-initM", 6)==0) {
						STR2Decimal(data+7, tmp);
						initM = tmp;
					}
				}
			} 
			else {
				auto & lst = XPP->_playList;
				lst.push_back(args[i].c_str());
				int idx = lst.size()-1;
				auto & added = lst.at(idx);
				if(added.EndWith(L".txt")) {
					lst.erase(lst.begin()+idx);
				} else if(added.GetLength()>5){
					if(added[0]==L'"') {
						added.Mid(1, added.GetLength()-2);
					}
				}
				//QkString path_ = path.c_str();
				//if(!path_.EndWith(L".txt")) { // todo opt
				//	//if (!append)
				//	//{
				//	//	_mainPlayer.PlayVideoFile(path_);
				//	//	append = true;
				//	//}
				//}
			}
		}
	}
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

	bool bNoWait = false;


	bool append = std::find(_args.begin(), _args.end(), L"-add")!= _args.end();
	for (size_t i = 0; i < _args.size(); i++)
	{
		auto &  path = _args[i];
		//lxx(ss, (path.data()))
		if(path.size()>0) {
			if(path[0]==L'-') {
				if(StrCmpN(_args[i].c_str(), L"-nowait", 7)==0) {
					bNoWait = true;
				}
			}
		}
	}


	if(!bNoWait) {
		// 防止短时间打开许多实例
		hMutexTemp = CreateMutex(NULL, TRUE, L"WODPLTMP1");
		if (GetLastError() == ERROR_ALREADY_EXISTS) // 检查互斥体是否已存在
		{
			if(prvInstance(lpCmdLine, TRUE)) return 0;
			hMutexTemp = 0;
		}
	}

	// 单实例播放器
	hMutexReady = CreateMutex(NULL, FALSE, L"WODPLTRDY");
	if (GetLastError() == ERROR_ALREADY_EXISTS) // 检查互斥体是否已存在
	{
		if(hMutexReady) { // 释放互斥体
			ReleaseMutex(hMutexReady);
			CloseHandle(hMutexReady);
			hMutexReady = 0;
		}
		if(prvInstance(lpCmdLine, FALSE)) return 0;
	}
	if(hMutexReady) { // 释放互斥体
		ReleaseMutex(hMutexReady);
		CloseHandle(hMutexReady);
	}


	// 初始化公共空间
	//INITCOMMONCONTROLSEX icc;
	//icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	//icc.dwICC = ICC_BAR_CLASSES;
	//InitCommonControlsEx(&icc);

	CPaintManagerUI::SetInstance(hInstance);
#ifdef _DEBUG
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("..//skin//"));
#else
	//CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);
	//CPaintManagerUI::SetResourceType(UILIB_FILE);
	//CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("..//skin//"));

	//CPaintManagerUI::SetResourceZip(_T("skin.zip"));
	//CPaintManagerUI::SetResourcePath(strResourcePath.GetData());

	CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);
	//CPaintManagerUI::SetResourcePath(_T("skin.zip"));
	
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
		//::FreeResource(hResource);
	}
#endif



	TCHAR usrDir[MAX_PATH];
	::GetModuleFileName(NULL, usrDir, MAX_PATH);
	::PathRemoveFileSpec(usrDir);
	//::PathAppend();

	ImmDisableIME(0);

	loadProf(usrDir, configFileName);

	initX=GetProfInt("initX", -1);
	initY=GetProfInt("initY", -1);
	initW=GetProfInt("initW", 500);
	initH=GetProfInt("initH", 500);


	XPP = new WODApplication();
	//lxx(ss dd, lpCmdLine, _args.size())
	if (_args.size()>0)
	{
		parseArgs(_args);
	}

	//WODApplication app{};
	//XPP = new WODApplication();
	//XPP = &app;
	XPP->Create(NULL, _T("无限播放器"), UI_WNDSTYLE_FRAME, WS_EX_APPWINDOW|WS_EX_ACCEPTFILES, initX, initY, initW, initH);
	XPP->ShowWindow();
	if(initX==-1 && initY==-1) // todo
		XPP->CenterWindow();
	XPP->buildAccelerator();
	//XPP->init(hInstance, NULL);
	//XPP->ShowModal();
	CControlUI* btn = new CControlUI();

	if(initM) {
		XPP->m_pm.GetRoot()->PostLambda([](){
			if(initM&0x1) {
				XPP->ToggleMini();
			}
			if(initM&0x2) {
				XPP->ToggleFullScreen();
			}
			//if(initM&0x2) {
			//	XPP->SendMessage(WM_SYSCOMMAND, XPP->_isMaximized=SC_MAXIMIZE, 0);
			//	XPP->m_pm._bStaticMovable = false; 
			//	XPP->m_pm.GetRoot()->SetInset(0);
			//}
			return false;
		}, 250);
	}

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
				case WM_SYSCOMMAND:
				{
					BOOL bHandled = TRUE;
					LRESULT lRes = XPP->HandleCustomMessage(msg.message, msg.wParam, msg.lParam, bHandled);
					//if (bHandled) return lRes;
				} break;
				case WM_SYSKEYDOWN :
				case WM_KEYDOWN:
					lxxx(WM_KEYDOWN dd, msg.wParam)
					if(msg.wParam==VK_CONTROL || msg.wParam==VK_SHIFT || msg.wParam==VK_MENU)
						{}
					else
						keyPressed = HIWORD(msg.lParam) & KF_REPEAT;
					break;
				//case WM_KEYUP:
				//	lxxx(WM_KEYUP dd dd, msg.wParam, msg.lParam)

				//	break;
				case WM_MOUSELEAVE:
				{
					//lxx(1)
					if(msg.hwnd==XPP->_topBarFscWnd->GetHWND()) {
						XPP->_topBarFscWnd->SetVisible(false);
					}
					if(msg.hwnd==XPP->_hFscBtmbar) {
						XPP->_bottomBar->SetVisible(false);
					}
				} break;
				case WM_MOUSEMOVE:
				//case WM_NCMOUSEMOVE:
					if(scheduleMoveWnd) {
						if((GetKeyState(VK_LBUTTON) & 0x8000) != 0)
						{
							::SendMessage(XPP->GetHWND(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
							break;
						}
						scheduleMoveWnd = 0;
					}
					if (XPP->_isFullScreen||XPP->_isMini)
						hookMouseMove(msg); // show hover bar
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

	if(hMutexReady) { // 释放互斥体
		ReleaseMutex(hMutexReady);
		CloseHandle(hMutexReady);
	}
	if(hMutexTemp) { // 释放互斥体
		ReleaseMutex(hMutexTemp);
		CloseHandle(hMutexTemp);
	}

}



