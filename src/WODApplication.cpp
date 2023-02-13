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
#include "WODMenus.h"


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

//#include "../WndControl/ButtonList.h"

CControlUI* WODApplication::CreateControl(LPCTSTR pstrClass){
	if(lstrcmp(pstrClass, L"seekbar")==0) {
		return SeekBar::CreateControl();
	}
	if(lstrcmp(pstrClass, L"seekbarwnd")==0) {
		//return new SeekBarOld();
	}
	if(lstrcmp(pstrClass, L"btnlst")==0) {
		//return new ButtonList();
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
	ResetWndOpacity();
	initWodMenus(this);

	//m_pm._bIsLayoutOnly = true;
	_playBtn = m_pm.FindControl(_T("play"));
	_bottomBar = static_cast<WinFrame*>(m_pm.FindControl(_T("ch1")));

	_topBar = static_cast<WinFrame*>(m_pm.FindControl(_T("topBar")));
	_topBarFscWnd = static_cast<WinFrame*>(m_pm.FindControl(_T("topW")));
	_topBarFscH = static_cast<WinFrame*>(m_pm.FindControl(_T("topH")));

	m_pm.GetShadow()->ShowShadow(true);
	m_pm.GetShadow()->SetSize(5);
	m_pm.GetShadow()->SetSharpness(9);
	m_pm.GetShadow()->SetPosition(1,1);
	m_pm.GetShadow()->SetColor(0x55888888);

	seekbar = static_cast<SeekBar*>(m_pm.FindControl(_T("seekbar")));
	if(seekbar)
		seekbar->_callback = (SeekBarTrackCallback)seekchange;

	_db->Init();

	auto file = GetProfString("file");
	QkString path = file?file->c_str():"";
	if(!path.IsEmpty())
		_mainPlayer.PlayVideoFile(STR(path));
	//MarkPlaying(true);

	//tg

	BOOL bHandled;
	//HandleCustomMessage(WM_KEYDOWN, VK_P, 0, bHandled);
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

//LRESULT WODApplication::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	return __super::OnCreate(uMsg, wParam, lParam, bHandled);
//}

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
		if (msg.sType==L"click")
		{
			//m_pm.SetDPI(125);

		}

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

void SetFloatHwnd(HWND hwnd)
{
	auto wndSty = GetWindowLong(hwnd, GWL_STYLE);
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
	SetParent(hwnd, 0);
	SetWindowLong(hwnd, GWL_STYLE, (wndSty & ~WS_CHILD ) | WS_POPUP);
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE)
		| WS_EX_LAYERED 
		| WS_EX_TOOLWINDOW
		| WS_EX_TOPMOST
	);
}


void WODApplication::ToggleFullScreen()
{
	HWND hThisFSc = m_hWnd;
	DWORD style = GetWindowLong(hThisFSc, GWL_STYLE);
	if (!_isFullScreen)
	{
		HWND _hFullScreenBtmbar = _bottomBar->GetHWND();
		_bottomBar->SetFloat(true);

		_topBar->GetParent()->Remove(_topBar);
		_topBarFscH->Add(_topBar);
		_topBarFscWnd->SetVisible(true);

		SetFloatHwnd(_hFullScreenBtmbar);
		SetFloatHwnd(_topBarFscH->GetHWND());

		::SetWindowPos(_hFullScreenBtmbar, HWND_TOPMOST, 0, 0, 100, 100, 0);
		::SetWindowPos(_topBarFscH->GetHWND(), HWND_TOPMOST, 0, 0, 100, 100, 0);

		SetLayeredWindowAttributes(_hFullScreenBtmbar, TransparentKey, 200, LWA_ALPHA);

		_isFullScreen = true;
		GetWindowRect(hThisFSc, &rcNScPos);
		style = GetWindowLong(hThisFSc, GWL_EXSTYLE);

		int w = ::GetSystemMetrics(SM_CXSCREEN);
		int h = ::GetSystemMetrics(SM_CYSCREEN);
		::SetWindowPos(hThisFSc, NULL, 0, 0, w, h, 0);

		_topBarFscWnd->SetVisible(false);
		_bottomBar->SetVisible(false);

		_bottomBar->SetFloat(true);
		m_pm.GetSizeBox().top = 0;
	}
	else
	{
		_bottomBar->SetFloat(false);
		SetParent(_mainPlayer._seekbar.GetHWND(), m_hWnd);
		_isFullScreen = false;
		style = GetWindowLong(hThisFSc, GWL_EXSTYLE);
		SetWindowLong(hThisFSc, GWL_EXSTYLE, style&~WS_EX_TOPMOST);

		::SetWindowPos(hThisFSc, NULL, rcNScPos.left, rcNScPos.top
			, rcNScPos.right-rcNScPos.left, rcNScPos.bottom-rcNScPos.top, 0);

		_topBar->GetParent()->Remove(_topBar);
		m_pm.GetRoot()->AddAt(_topBar, 0);
		_bottomBar->SetVisible(true);
		_topBarFscWnd->SetVisible(false);
		m_pm.GetSizeBox().top = 4;
	}
}

bool WODApplication::IsFullScreen()
{
	return (GetWindowLong(m_hWnd, GWL_STYLE)&WS_CAPTION)==0;
	//return _isFullScreen;
}

bool _playing = false;

void WODApplication::MarkPlaying(bool playing)
{
	if(_playing!=playing)
	{
		_playing=playing;
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
		if (_mainPlayer.PlayVideoFile(filepath))
		{
			QkString filePath = filepath;
			PutProfString("file", filePath.GetData(threadBuffer));
		}
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


void Replay()
{
	::KillTimer(XPP->GetHWND(), 10086);
	XPP->_mainPlayer.Stop();
	XPP->MarkPlaying(false);
	XPP->_mainPlayer.Release();
	XPP->MarkPlaying(false);
	nxt_file[0] = 0;
	::SetTimer(XPP->GetHWND(), 10086, 100, 0);
}

void SeekDelta(int delta, int level)
{
	auto & player = XPP->_mainPlayer._mMediaPlayer;
	if (player)
	{
		int max=player->GetDuration();
		int factor = 1;
		if(level==1) {
			factor = 5;
		}
		if(level==2) {
			factor = 30;
		}
		//LogIs(2, "set= %d", delta*factor*1000);
		delta = player->GetPosition()+delta*factor*1000;
		bool set=0;
		if(delta<0)  delta=0;
		else if(delta>max) delta=max;
		else set=1;
		if(set || player->GetPosition()!=delta) {
			player->SetPosition(delta);
		}
		if(!XPP->_mainPlayer._isPlaying && !XPP->_mainPlayer._seekbar._isSeeking) {
			XPP->_mainPlayer._seekbar.SetProgressAndMax(delta, player->GetDuration());
		}
	}
}

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
			::KillTimer(m_hWnd, 10086);
			if(nxt_file[0]!=0)
			{
				if (_mainPlayer.PlayVideoFile(nxt_file))
				{
					QkString filePath = nxt_file;
					PutProfString("file", filePath.GetData(threadBuffer));
				}
			}
			else
				_mainPlayer.PlayVideoFile(_mainPlayer._currentPath);
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
		//case VK_C:
		//{
		//	//if(IsKeyDown(VK_CONTROL)) {
		//	//	VLCPlayer* player = (VLCPlayer*)_mainPlayer._mMediaPlayer;
		//	//	player->takeSnapShot("G:\\IMG\\tmp.png");
		//	//}
		//	//int pos = _mainPlayer._mMediaPlayer->GetPosition();
		//}
		//break;
		default:
			break;
		}
	case WM_COMMAND:
	{
		bHandled = true;
		switch (LOWORD(wParam))
		{
		case IDM_PLAY:
			_mainPlayer.Toggle();
			break;
		case IDM_FILE_CLOSE:
		case IDM_STOP:
			if (_mainPlayer._mMediaPlayer)
				_mainPlayer._mMediaPlayer->Stop();
			MarkPlaying(false);
			break;
		case IDM_FILE_OPEN:
		case IDM_OPEN:
			PickFile();
			break;
		case IDM_MIN:
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
			break;
		case IDM_MAXMISE:
			if(_isFullScreen) 
			{
				ToggleFullScreen();
				SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); 
				break;
			}
		case IDM_MAX:
			if(_isFullScreen) 
				ToggleFullScreen();
			else if(::IsMaximized(GetHWND()))
				SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); 
			else
				SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); 
			break;
		case IDM_CLOSE:
			Close();
			break;

		case IDM_BKMK_ADD:
		{
			//WOD_IMG_UTILS("screenshotie", _mainPlayer._mMediaPlayer->getHWND());
			_mainPlayer.AddBookmark();
		} break;

		case IDM_SHUTDOWN:
			Close();
			break;
		case IDM_PAUSE:
			_mainPlayer.Toggle();
			break;
		case IDM_SEEK_FORE: SeekDelta(1, 0); break;
		case IDM_SEEK_FORE_FAST: SeekDelta(1, 1); break;
		case IDM_SEEK_FORE_FASTER: SeekDelta(1, 2); break;
		case IDM_SEEK_BACK: SeekDelta(-1, 0); break;
		case IDM_SEEK_BACK_FAST: SeekDelta(-1, 1); break;
		case IDM_SEEK_BACK_FASTER: SeekDelta(-1, 2); break;

		case IDM_SPEED_UP:  _mainPlayer.SpeedDelta(0.1f); break;
		case IDM_SPEED_DOWN:  _mainPlayer.SpeedDelta(-0.1f); break;
		case IDM_SPEED_UP_FAST:  _mainPlayer.SpeedDelta(0.5f); break;
		case IDM_SPEED_DOWN_FAST:  _mainPlayer.SpeedDelta(-0.5f); break;
		case IDM_SPEED_UP_FASTER:  _mainPlayer.SpeedDelta(1); break;
		case IDM_SPEED_DOWN_FASTER:  _mainPlayer.SpeedDelta(-1); break;
		case IDM_SPEED_RESET: _mainPlayer.SpeedDelta(0); break;

		case IDM_FILE:
		case IDM_BKMK:
		case IDM_SKIN:
		case IDM_PLUGIN:
			trackWodMenus((CControlUI*)lParam, wParam);
			break;

		case IDM_PLUGIN_MF:
			PutProfString("player", "MFExternalPlayer.dll");
			Replay();
			return 1;
		case IDM_PLUGIN_VLC:
			PutProfString("player", "VLCExternalPlayer.dll");
			Replay();
			return 1;
		case IDM_PLUGIN_XL:
			PutProfString("player", "XunLeiExternalPlayer\\XunLeiExternalPlayer.dll");
			Replay();
			return 1;

		case IDM_SKIN_NORM:
		case IDM_SKIN_HOLLOW:
		case IDM_SKIN_ALPHA_1 :
		case IDM_SKIN_ALPHA_2 :
		case IDM_SKIN_ALPHA_3 :
		case IDM_SKIN_ALPHA_4 :
		case IDM_SKIN_ALPHA_5 :
		case IDM_SKIN_ALPHA_6 :
		case IDM_SKIN_ALPHA_7 :
		case IDM_SKIN_ALPHA_8 :
		case IDM_SKIN_ALPHA_9 :
		case IDM_SKIN_ALPHA_10:
			PutProfInt("WndOp", wParam==IDM_SKIN_NORM?0
				:wParam==IDM_SKIN_HOLLOW?1
				:2);
			if(wParam>IDM_SKIN_HOLLOW) 
			{
				PutProfInt("WndAlpha", 10*(wParam-(IDM_SKIN_HOLLOW)));
			}
			ResetWndOpacity();
			return 1;
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

void WODApplication::ResetWndOpacity()
{
	// 0:normal 1:hollow 2:transparentz
	_WndOp = GetProfInt("WndOp", 0);
	if(_WndOp==0) 
	{
		SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
	}
	else 
	{
		SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		//int WndOp = GetProfInt("WndOp", 0);
		TransparentKey = RGB(0,1,0);
		int alpha = GetProfInt("WndAlpha", 100);
		if(_WndOp==1) 
		{
			SetLayeredWindowAttributes(m_hWnd, TransparentKey, 0, LWA_COLORKEY);
		}
		else
		{
			SetLayeredWindowAttributes(m_hWnd, TransparentKey, ceil(alpha/100.f*255), LWA_ALPHA);
		}
	}
}
