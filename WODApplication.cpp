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
#include <pch.h>
#include "../resource.h"
#include "database\database_helper.h"
#include "virtual_keys.h"


struct DemoData
{
	const TCHAR* title;
	int image;
};

extern WODApplication* XPP;

extern int testSqlite();

SeekBar* seekbar;

WODApplication::WODApplication()
{
	_db = new WODBase();
	_frameLess = 1;
	//_roundwnd = 1;
}

#include "../WndControl/ButtonList.h"

CControlUI* WODApplication::CreateControl(LPCTSTR pstrClass){
	if(lstrcmp(pstrClass, L"seekbar")==0) {
		return SeekBar::CreateControl();
	}
	if(lstrcmp(pstrClass, L"seekbarwnd")==0) {
		//return new SeekBarOld();
	}
	if(lstrcmp(pstrClass, L"btnlst")==0) {
		return new ButtonList();
	}
	if(*pstrClass=='_') {
		pstrClass++;
		if(*pstrClass=='s') {
			//LogIs(L"CreateControl::%s %d", pstrClass, *pstrClass=='_');
			if(_mainPlayer._seekbar.GetParent()==NULL)
				return &_mainPlayer._seekbar;
			return &_mainPlayer._volumebar;
		}
		if(*pstrClass=='v') {
			//LogIs(L"CreateControl::%s %d", pstrClass, *pstrClass=='_');
			_mainPlayer._app = this;
			return &_mainPlayer;
		}
	}
	return 0;
}


void seekchange(SeekBar* bar, int pos) {
	XPP->_mainPlayer._mMediaPlayer->SetPosition(pos);
}


void WODApplication::InitWindow()
{
	//m_pm._bIsLayoutOnly = true;
	_playBtn = m_pm.FindControl(_T("play"));

	TransparentKey = RGB(0,1,0);

	m_pm.GetShadow()->ShowShadow(true);
	m_pm.GetShadow()->SetSize(5);
	m_pm.GetShadow()->SetSharpness(9);
	m_pm.GetShadow()->SetPosition(1,1);
	m_pm.GetShadow()->SetColor(0x55888888);

	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(m_hWnd, TransparentKey, 0, LWA_COLORKEY);

	//ListView* pList = static_cast<ListView*>(m_pm.FindControl(_T("btn")));
	//LogIs(L"WODApplication::InitWindow %s", (LPCWSTR)m_pm.FindControl(L"btn")->GetText());

	//CHorizontalLayoutUI* menuBar = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("menuBar")));
	//for (size_t i = 0; i < 10; i++)
	//{
	//    auto menu = builder.Create(L"menu_item.xml", 0, 0, &m_pm);
	//    menu->SetFixedWidth(0);
	//    menu->GetText().Format(L"菜单#%d", i);
	//    menu->GetText().Format(L"文件#%d", i);
	//    menu->GetText().Format(L"文件(&F)", i);
	//    menuBar->Add(menu);
	//}

	seekbar = static_cast<SeekBar*>(m_pm.FindControl(_T("seekbar")));
	if(seekbar)
		seekbar->_callback = (SeekBarTrackCallback)seekchange;

	_mainPlayer.newVideoView();


	_db->Init();

	BOOL bHandled;
	//HandleCustomMessage(WM_KEYDOWN, VK_P, 0, bHandled);

	//QkString test="asdsad";
	//CHAR fullpath[_MAX_PATH*2];
	//WideCharToMultiByte(CP_ACP, 0, test, -1, fullpath, _MAX_PATH*2, 0, 0);
	//LogIs(2, fullpath);

	//tg
}

LRESULT WODApplication::OnClose(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	//LogIs("OnClose");
	::DestroyWindow(GetHWND());
	bHandled = TRUE;
	return 0;
}

LRESULT WODApplication::OnDestroy( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled )
{
	//LogIs("OnDestroy");
	bHandled = TRUE;
	_mainPlayer.Release();
	if(_db){
		delete _db;
		_db = 0;
	}
	::PostQuitMessage(wParam);
	return 0;
}

void WODApplication::OnFinalMessage( HWND hWnd )
{
	__super::OnFinalMessage(hWnd);
	delete this;
}



void WODApplication::Notify( TNotifyUI &msg )
{
	//if (msg.sType==DUI_MSGTYPE_BUTTONDOWN)
	//{
	//	LogIs(2, "12");
	//	if(msg.pSender->m_bStatic)
	//	{
	//		ReleaseCapture();
	//		::SendMessage(XPP->GetHWND(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
	//		SetFocus(XPP->GetHWND());
	//	}
	//	//auto bRoot = builder.Create(L"<Window><Button/></Window>", TEXT("str"), 0, &m_pm);
	//	//ASSERT(bRoot);
	//}
	if (msg.sType==L"click")
	{
		if( msg.sType == _T("itemclick") ) 
		{
		}
		//auto bRoot = builder.Create(L"<Window><Button/></Window>", TEXT("str"), 0, &m_pm);
		//ASSERT(bRoot);
	}
	// WindowImplBase::Notify(msg);
}

HBRUSH bgBrush;

void WOD_Register(HINSTANCE hInstance)
{
	bgBrush = CreateSolidBrush (RGB(29,29,29));
	wchar_t lpszMenuName[] = L"Menu";

	//static TCHAR szAppName[] = TEXT("Hello");
	//MSG    msg;
	//WNDCLASS wndclass;//WNDCLASSEX比WNDCLASS多两个结构成员－－cbSize(指定WNDCLASSEX结构的大小－－字节)  －－hIconSm(标识类的小图标)
	//wndclass.style = CS_HREDRAW | CS_VREDRAW;
	//wndclass.lpfnWndProc = WODApplication::WndProc;
	//wndclass.cbClsExtra = 0;
	//wndclass.cbWndExtra = 0;
	//wndclass.hInstance = hInstance;
	//wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wndclass.hbrBackground = (HBRUSH)(COLOR_MENU + 1);//白色 COLOR_WINDOW // COLOR_MENU 界面灰
	//												  //wndclass.hbrBackground = bgBrush;
	//												  //wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//												  //wndclass.hbrBackground = CreateSolidBrush(RGB(0, 1, 0));
	//wndclass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	//wndclass.lpszMenuName = lpszMenuName;
	//wndclass.lpszMenuName = 0;
	//wndclass.lpszClassName = szAppName;

	//RegisterClass(&wndclass);
}


static DWORD dwNScStyle = WS_CAPTION|WS_BORDER|WS_SIZEBOX; 
RECT rcNScPos;


LRESULT WINAPI FullScreenBarsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void WODApplication::ToggleFullScreen()
{
	HWND hThisFSc = m_hWnd;
	DWORD style = GetWindowLong(hThisFSc, GWL_STYLE);
	if (style&WS_CAPTION)
		//if (!_isFullScreen)
	{
		if(_hFullScreenBtmbar == NULL)
		{
			// Register window class
			WNDCLASS wc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			//wc.hbrBackground = CreateSolidBrush (RGB(112,112,255));
			wc.hbrBackground = bgBrush;
			wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
			wc.hIcon = NULL;
			wc.hInstance = CPaintManagerUI::GetInstance();
			wc.lpfnWndProc = FullScreenBarsProc;
			wc.lpszClassName = TEXT("FullScreenBarsHolder");
			wc.lpszMenuName = 0;
			wc.style = CS_GLOBALCLASS | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;

			if(!::RegisterClass(&wc))
			{
				int nError = GetLastError();
				if(nError != ERROR_CLASS_ALREADY_EXISTS)
					return;
			}

			_hFullScreenBtmbar = ::CreateWindow(wc.lpszClassName
				, TEXT(""), WS_VISIBLE | WS_CHILD
				, 0, 0, 100, 100,
				m_hWnd, NULL, NULL, this);
		}
		//SetParent(_toolbar.GetHWND(), _hFullScreenBtmbar);
		SetParent(_mainPlayer._seekbar.GetHWND(), _hFullScreenBtmbar);

		if (_mainPlayer._mMediaPlayer) _mainPlayer._mMediaPlayer->SetFullScreen(true);
		_isFullScreen = true;
		GetWindowRect(hThisFSc, &rcNScPos);
		SetWindowLong(hThisFSc, GWL_STYLE , style&~dwNScStyle );
		style = GetWindowLong(hThisFSc, GWL_EXSTYLE);
		SetWindowLong(hThisFSc, GWL_EXSTYLE, style|WS_EX_TOPMOST);

		int w = ::GetSystemMetrics(SM_CXSCREEN);
		int h = ::GetSystemMetrics(SM_CYSCREEN);
		::SetWindowPos(hThisFSc, NULL, 0, 0, w, h, 0);
		ShowWindow(_hFullScreenBtmbar, SW_HIDE);
	}
	else
	{
		//SetParent(_toolbar.GetHWND(), m_hWnd);
		SetParent(_mainPlayer._seekbar.GetHWND(), m_hWnd);
		ShowWindow(_hFullScreenBtmbar, SW_HIDE);
		if (_mainPlayer._mMediaPlayer) _mainPlayer._mMediaPlayer->SetFullScreen(false);
		_isFullScreen = false;
		SetWindowLong(hThisFSc, GWL_STYLE , style|dwNScStyle );
		style = GetWindowLong(hThisFSc, GWL_EXSTYLE);
		SetWindowLong(hThisFSc, GWL_EXSTYLE, style&~WS_EX_TOPMOST);

		::SetWindowPos(hThisFSc, NULL, rcNScPos.left, rcNScPos.top
			, rcNScPos.right-rcNScPos.left, rcNScPos.bottom-rcNScPos.top, 0);
	}
}

bool WODApplication::IsFullScreen()
{
	return (GetWindowLong(m_hWnd, GWL_STYLE)&WS_CAPTION)==0;
	//return _isFullScreen;
}

void WODApplication::MarkPlaying(bool playing)
{
	_playBtn->SetForeImage(playing?L"pause.png":L"play.png");
	//RECT rc;
	//GetClientRect(_toolbar.GetHWND(), &rc);
	//rc.right = rc.bottom;
	//InvalidateRect(_toolbar.GetHWND(), &rc, TRUE);

	if(playing)
		::SetTimer(m_hWnd, 1, 100, NULL);
	else
		::KillTimer(m_hWnd, 1);
}

extern bool running;


BOOL PickFileDlg(HWND hOwner,
	BOOL bIsSave,
	const TCHAR * pcszTitle,
	const TCHAR * pcszFilter,
	TCHAR * pszFilePath,
	int nMaxFilePath,
	TCHAR * pszFileTitle,
	int nMaxFileTitle)
{
	OPENFILENAME ofn;

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hOwner;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = pcszFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = pszFilePath;
	ofn.nMaxFile = nMaxFilePath;
	ofn.lpstrFileTitle = pszFileTitle;
	ofn.nMaxFileTitle = nMaxFileTitle;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = pcszTitle;
	ofn.Flags = OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;

	BOOL bResult = 0;
	if(bIsSave)
		bResult = GetSaveFileName(&ofn);
	else
		bResult = GetOpenFileName(&ofn);
	return bResult;
}

bool WODApplication::PickFile()
{
	//if(1) {
	//	m_pm.SetDPI(125);
	//	return 1;
	//}
	TCHAR filepath[MAX_PATH]{};

	// Build filter
	TCHAR formats[512];
	lstrcpy(formats, TEXT(""));
	lstrcat(formats, TEXT("*.wmv;*.wmp;*.wm;*.asf;*.wma;*.avi;*.wav;*.mpg;*.mpeg;*.dat;"));
	lstrcat(formats, TEXT("*.ts;*.mpa;*.mp2;*.vob;*.ifo;*.mp3;*.mid;*.ogm;*.ogg;*.cda;"));
	lstrcat(formats, TEXT("*.d2v;*.mp4;*.3gp;*.mkv;*.rm;*.ram;*.rmvb;*.rpm;*.ra;*.mov;"));
	lstrcat(formats, TEXT("*.qt;*.amr;*.mpc;*.flv;*.swf;"));
	lstrcat(formats, TEXT("*.evo;*.tta;*.m4b;"));
	lstrcat(formats, TEXT("*.xv;*.xvx;*.xlmv"));

	TCHAR filters[512];
	TCHAR * pFilter = filters;
	wsprintf(pFilter, TEXT("全部支持的媒体(%s)"), formats);
	pFilter += (lstrlen(pFilter) + 1);
	lstrcpy(pFilter, formats);
	pFilter += (lstrlen(pFilter) + 1);
	lstrcpy(pFilter, TEXT("所有文件(*.*)"));
	pFilter += (lstrlen(pFilter) + 1);
	lstrcpy(pFilter, TEXT("*.*"));
	pFilter += (lstrlen(pFilter) + 1);
	*pFilter = 0; pFilter++;

	BOOL ret = PickFileDlg(m_hWnd, FALSE, TEXT("选择媒体文件"), filters, filepath, MAX_PATH, NULL, 0);

	if(ret)
	{
		_mainPlayer.PlayVideoFile(filepath);
	}
	return false;
}

bool IsKeyDown(int key) {
	return (::GetKeyState(key) & 0x80000000) != 0;
}


#include <Windows.h>
#include <GdiPlus.h>
#pragma comment(lib, "GdiPlus")//Visual Studio specific


TCHAR nxt_file[_MAX_PATH];
//std::string threadBuffer;


LRESULT WODApplication::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
	case WM_DROPFILES:
	{
		HDROP hDropInfo = (HDROP)wParam;
		UINT  nFileCount = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
		TCHAR szFileName[_MAX_PATH] = TEXT("");
		DWORD dwAttribute;

		// 获取拖拽进来文件和文件夹
		for (UINT i = 0; i < nFileCount; i++)
		{
			::DragQueryFile(hDropInfo, i, szFileName, sizeof(szFileName));
			dwAttribute = ::GetFileAttributes(szFileName);

			// 是否为文件夹
			if (dwAttribute & FILE_ATTRIBUTE_DIRECTORY)
			{          
			}
			else
			{
				// filepath
				if (_mainPlayer._mMediaPlayer)
				{
					_mainPlayer._mMediaPlayer->Close();
					lstrcpy(nxt_file, szFileName);
					::KillTimer(m_hWnd, 10086);
					::SetTimer(m_hWnd, 10086, 200, 0);
				}
				//CHAR buffer[256]={0};
				//WideCharToMultiByte (CP_ACP, 0, szFileName
				//	, -1, buffer, 256, 0, 0) ;

				MarkPlaying();
			}
		}

		::DragFinish(hDropInfo);
		return true;
	};
	break;


	case WM_SETFOCUS:
	{
		LogIs(L"WM_SETFOCUS");
	} break;
	case WM_KILLFOCUS:
	{
		LogIs(L"WM_KILLFOCUS");
		if(_mainPlayer.IsMediaPlayerWindow(::GetFocus())) {
			::SetFocus(m_hWnd);
		}
	} break;

	case WM_CONTEXTMENU:
	{
		return 0;
	}
	//case WM_PAINT: case WM_CLOSE: case WM_DESTROY:
	//case WM_SIZE: // old layout code see ::  https://github.com/KnIfER/WODPlayer/blob/9a19f3e5f0893ba82c1a8a3566e5b55f7a3e6290/src/WndControl/WODWindow.cpp#L425

	case WM_TIMER:
	{
		if (wParam==10086)
		{
			_mainPlayer.PlayVideoFile(nxt_file);
			::KillTimer(m_hWnd, 10086);
		}
		if ((wParam == 1)
			&& (_mainPlayer._mMediaPlayer->IsPlaying() || _mainPlayer._mMediaPlayer->IsPaused()) )
		{
			long pos = _mainPlayer._mMediaPlayer->GetPosition();

			//TCHAR szPosition[64];
			//TCHAR szDuration[64];
			//g_MyPlayer.MillisecondToText(g_MyPlayer.m_nPosition, szPosition);
			//g_MyPlayer.MillisecondToText(g_MyPlayer.m_nDuration, szDuration);

			//lstrcat(szPosition, _T("/"));
			//lstrcat(szPosition, szDuration);

			int nPos =  int (pos / (double)_mainPlayer._mMediaPlayer->GetDuration() * 1000);
			//SendMessage(GetDlgItem(hwnd, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)nPos);

			if (!_mainPlayer._seekbar._isSeeking)
			{
				_mainPlayer._seekbar.SetProgressAndMax(pos, _mainPlayer._mMediaPlayer->GetDuration());
			}

			_mainPlayer._mMediaPlayer->syncResolution();
			//LogIs(3, "setPosition:: %d %d max=%d curr=%d\n", _mainPlayer._mMediaPlayer->m_nPosition, _mainPlayer._mMediaPlayer->m_nDuration, _mainPlayer._seekbar.GetMax(), _mainPlayer._seekbar.GetPosition());
		}
	}
	return 0;

	case WM_HSCROLL:
	{
		//LogIs("SetTime GETPOS:: %ld \n", _mainPlayer._seekbar.GetPosition());
		if ((HWND)lParam==_mainPlayer._seekbar.GetHWND())
		{
			if (_mainPlayer._seekbar._isSeeking && _mainPlayer._mMediaPlayer
				&& (_mainPlayer._mMediaPlayer->IsPlaying() || _mainPlayer._mMediaPlayer->IsPaused()) )
			{
				_mainPlayer._mMediaPlayer->SetPosition(wParam);
			}
		}
	}
	break;

	case WM_QUIT:
		return 0;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_SPACE:
			_mainPlayer.Toggle();
			break;
		case VK_C:
		{
			//if(IsKeyDown(VK_CONTROL)) {
			//	VLCPlayer* player = (VLCPlayer*)_mainPlayer._mMediaPlayer;
			//	player->takeSnapShot("G:\\IMG\\tmp.png");
			//}
			//int pos = _mainPlayer._mMediaPlayer->GetPosition();
		}
		break;
		case VK_P:
		{
			//WOD_IMG_UTILS("screenshotie", _mainPlayer._mMediaPlayer->getHWND());

			_mainPlayer.AddBookmark();
		}
		break;
		case VK_LEFT:
		case VK_RIGHT:
			if (_mainPlayer._mMediaPlayer)
			{
				int delta = wParam==VK_LEFT?-1:1, max=_mainPlayer._mMediaPlayer->GetDuration();
				int factor = 1;
				if(IsKeyDown(VK_CONTROL)) {
					factor = 5;
				}
				if(IsKeyDown(VK_SHIFT)) {
					factor = 30;
				}
				delta = _mainPlayer._mMediaPlayer->GetPosition()+delta*factor*1000;
				bool set=0;
				if(delta<0)  delta=0;
				else if(delta>max) delta=max;
				else set=1;
				if(set || _mainPlayer._mMediaPlayer->GetPosition()!=delta) {
					_mainPlayer._mMediaPlayer->SetPosition(delta);
				}
				if(!_mainPlayer._isPlaying && !_mainPlayer._seekbar._isSeeking) {
					_mainPlayer._seekbar.SetProgressAndMax(delta, _mainPlayer._mMediaPlayer->GetDuration());
				}
			}
			break;
		default:
			break;
		}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDM_PLAY:
			_mainPlayer.Toggle();
			break;
		case IDM_STOP:
			if (_mainPlayer._mMediaPlayer)
				_mainPlayer._mMediaPlayer->Stop();
			MarkPlaying(false);
			break;
		case IDM_OPEN:
			PickFile();
			break;
		case IDM_MIN:
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
			break;
		case IDM_MAX:
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); 
			break;
		case IDM_CLOSE:
			Close();
			break;
		}

	}
	break;
	case WM_NOTIFY:
	{
		switch (((NMHDR FAR *) lParam)->code) 
		{
		case PSN_HELP:
		{
			break;
		}
		}
	}
	break;
	}

	return 0;
}
