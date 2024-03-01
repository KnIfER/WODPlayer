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


#include <thread>
#include <chrono>


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
		if(*pstrClass=='s') { // "_s"
			//LogIs(L"CreateControl::%s %d", pstrClass, *pstrClass=='_');
			if(_mainPlayer._seekbar.GetParent()==NULL)
				return &_mainPlayer._seekbar;
			if(_mainPlayer._seekfloat.GetParent()==NULL)
				return &_mainPlayer._seekfloat;
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

DWORD initTimer;
DWORD initTimerID = 3;
DWORD initTimerInterval = 500;
BOOL _singleInstance= 1;

void initTimerProc(HWND hwnd, UINT, UINT_PTR, DWORD)
{
	::KillTimer(hwnd, initTimerID);
	if (hMutexTemp && !_singleInstance)
	{
		ReleaseMutex(hMutexTemp);
		CloseHandle(hMutexTemp);
		hMutexTemp = 0;
	}
}

float fastForwardRate = 0;
BOOL fastForwardStill = False;
DWORD ffTimer = 0;
DWORD ffTimerID = 7;
DWORD ffTimerInterval = 50;

void ffTimerProc(HWND hwnd, UINT, UINT_PTR, DWORD)
{
	if (fastForwardRate!=0)
	{
		auto & player = *XPP->_mainPlayer._mMediaPlayer;
		if(fastForwardStill || XPP->_mainPlayer._isPlaying) {
			player.SetPosition(player.GetPosition() + fastForwardRate*1000, true);
		}
	}
	else {
		::KillTimer(hwnd, ffTimerID);
		ffTimer = 0;
	}
}

DWORD delTimerID = 4;
BOOL delPermanent = 4;
void deleteFileProc(HWND hwnd, UINT, UINT_PTR, DWORD)
{
	::KillTimer(hwnd, delTimerID);
	XPP->DeleteCurrentFile(delPermanent);
}

CControlUI* _timeLabel;
CControlUI* _durationLabel;

void WODApplication::InitWindow()
{
	ResetWndOpacity();
	initWodMenus(this);

	m_pm._bIsLayoutOnly = true;
	_playBtn = m_pm.FindControl(_T("play"));
	_bottomBar = static_cast<WinFrame*>(m_pm.FindControl(_T("ch1")));
	_titleBar = static_cast<WinFrame*>(m_pm.FindControl(_T("titleBar")));
	_driveTag = static_cast<WinFrame*>(m_pm.FindControl(_T("drive")));
	_mimeTag = static_cast<WinFrame*>(m_pm.FindControl(_T("mime")));

	_topBar = static_cast<WinFrame*>(m_pm.FindControl(_T("topBar")));
	_topBarFscWnd = static_cast<WinFrame*>(m_pm.FindControl(_T("topW")));
	_topBarFscH = static_cast<WinFrame*>(m_pm.FindControl(_T("topH")));

	_timeLabel = static_cast<CControlUI*>(m_pm.FindControl(_T("time")));
	_durationLabel = static_cast<CControlUI*>(m_pm.FindControl(_T("duration")));

	m_pm.GetShadow()->ShowShadow(true);
	m_pm.GetShadow()->SetSize(5);
	m_pm.GetShadow()->SetSharpness(9);
	m_pm.GetShadow()->SetPosition(1,1);
	m_pm.GetShadow()->SetColor(0x55888888);

	_db->Init();

	auto file = GetProfString("file");
	QkString path = file?file->c_str():"";
	if(!path.IsEmpty())
		_mainPlayer.PlayVideoFile(STR(path));
	if (_playList.size()>0)
	{
		_mainPlayer.PlayVideoFile(_playList[0]);
	}
	//MarkPlaying(true);

	//tg

	BOOL bHandled;
	//HandleCustomMessage(WM_KEYDOWN, VK_P, 0, bHandled);


	//string* player = GetProfString("player");
	//auto brand = player && *player=="VLCExternalPlayer.dll"?"VLC":"XUN";
	//CControlUI* menuBtn = (CControlUI*)m;
	//menuBtn->GetText().Empty();
	//menuBtn->GetText().Append(L"插件 ");
	//menuBtn->GetText() += brand;
	//menuBtn->SetNeedAutoCalcSize();

	ImmAssociateContext(GetHWND(), NULL);

	//m_pm.SetTimer(m_pm.GetRoot(), 0, 350, true);
	initTimer = ::SetTimer(GetHWND(), initTimerID, initTimerInterval, (TIMERPROC)initTimerProc);


	// 启动置顶
	auto hwnd = XPP->GetHWND();
	//auto wndSty = GetWindowLong(hwnd, GWL_STYLE);
	//SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TOPMOST);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 100, 100, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 100, 100, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);

}

LRESULT WODApplication::OnClose(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	isClosing = true;
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
		//LogIs(2, "12");
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
		//LogIs(2, "click");
		if (msg.sType==L"click")
		{
			//m_pm.SetDPI(125);

		}

		if (msg.pSender==_timeLabel)
		{
			_mainPlayer._seekfloat.SetVisible(!_mainPlayer._seekfloat.IsVisible());
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


static DWORD dwNScStyle = WS_CAPTION|WS_THICKFRAME; 
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
	HWND hWnd = m_hWnd;
	DWORD style = GetWindowLong(hWnd, GWL_STYLE);
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
		GetWindowRect(hWnd, &rcNScPos);

		style = GetWindowLong(hWnd, GWL_STYLE);
		SetWindowLong(hWnd, GWL_STYLE , style & ~dwNScStyle | WS_POPUP );

		//style = GetWindowLong(hWnd, GWL_EXSTYLE);
		//dwNScStyle = WS_EX_DLGMODALFRAME |
		//	WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE;
		//SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);

		_topBarFscWnd->SetVisible(false);
		_bottomBar->SetVisible(false);

		_bottomBar->SetFloat(true);
		m_pm.GetSizeBox().top = 0;

		int w = ::GetSystemMetrics(SM_CXSCREEN);
		int h = ::GetSystemMetrics(SM_CYSCREEN);
		::SetWindowPos(hWnd, NULL, 0, 0, w, h, 0);

		//HMONITOR hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
		//MONITORINFO mi = { sizeof(mi) };
		//if (GetMonitorInfo(hmon, &mi))
		//{
		//	::SetWindowPos(hWnd, HWND_TOPMOST, mi.rcMonitor.left,
		//		mi.rcMonitor.top,
		//		mi.rcMonitor.right - mi.rcMonitor.left,
		//		mi.rcMonitor.bottom - mi.rcMonitor.top, 0);
		//}
	}
	else
	{
		_bottomBar->SetFloat(false);
		SetParent(_mainPlayer._seekbar.GetHWND(), m_hWnd);
		_isFullScreen = false;
		style = GetWindowLong(hWnd, GWL_STYLE);
		SetWindowLong(hWnd, GWL_STYLE , style | WS_THICKFRAME );

		::SetWindowPos(hWnd, NULL, rcNScPos.left, rcNScPos.top
			, rcNScPos.right-rcNScPos.left, rcNScPos.bottom-rcNScPos.top, 0);

		_topBar->GetParent()->Remove(_topBar);
		m_pm.GetRoot()->AddAt(_topBar, 0);
		_bottomBar->SetVisible(true);
		_topBarFscWnd->SetVisible(false);
		m_pm.GetSizeBox().top = 4;
	}
	bool b1 = _isFullScreen||_isMaximized==SC_MAXIMIZE;
	m_pm.GetRoot()->SetInset(b1?0:5);
	m_pm._bStaticMovable = !b1; 
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
		//else
		//	::KillTimer(m_hWnd, 1);
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
			PutProfString("file", "");
		}
	}
	return false;
}

#include <Windows.h>
#include <GdiPlus.h>
#pragma comment(lib, "GdiPlus")//Visual Studio specific


TCHAR nxt_file[_MAX_PATH];
//std::string threadBuffer;


bool IsKeyDown(int key) {
	return (::GetKeyState(key) & 0x80000000) != 0;
}


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
		fastForwardRate = 0;
		int max=player->GetDuration();
		float factor = 1;
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
			player->SetPosition(delta, true);
		}
		if(!XPP->_mainPlayer._isPlaying && !XPP->_mainPlayer._seekbar._isSeeking) {
			XPP->_mainPlayer._seekbar.SetProgressAndMax(delta, player->GetDuration());
		}
	}
}

void NavTimemark(int delta)
{
	auto & wod = XPP->_mainPlayer;
	if(!wod._mMediaPlayer) return;
	auto & player = wod._mMediaPlayer;
	auto & bkmks = wod._bookmarks;
	int pos = player->IsPlaying()?wod._seekbar._progress:player->GetPosition(); // todo
	if(bkmks.size())
	{
		bool set = false;
		for (size_t i = 0, size=bkmks.size(); i < size; i++)
		{
			int mark = bkmks[i].pos;
			if (delta<0)
			{
				if (mark >= pos)
				{
					if (i>0)
					{
						while(bkmks[i].layer!=0 && i>0 && bkmks[i-1].pos==mark) {
							i--;
						}
						wod.SelectBookMark(i-1);
						set = 1;
					}
					break;
				}
			}
			else if (mark > pos)
			{
				while(bkmks[i].layer!=0 && i+1 < size && bkmks[i+1].pos==mark) {
					i++;
				}
				wod.SelectBookMark(i);
				set = 1;
				break;
			}
		}
		if (!set)
		{
			//wod.SelectBookMark(delta<0?bkmks.size()-1:0);
			if (delta<0 && bkmks[bkmks.size()-1].pos < pos)
			{
				wod.SelectBookMark(bkmks.size()-1);
				set = 1;
			}
		}
		//lxx(dd dd dd, bkmks[wod._selectedBookmark].layer
		//	, bkmks[wod._selectedBookmark].pos
		//	, wod._selectedBookmark+1<bkmks.size()?bkmks[wod._selectedBookmark+1].pos:-1
		//)
		wod._seekbar.Invalidate();
	}
}

//std::function<bool (void)> haha = [](){return 0;};

void NavTime(int t)
{
	auto & wod = XPP->_mainPlayer;
	if(!wod._mMediaPlayer) return;
	auto & player = wod._mMediaPlayer;
	LONG pos = t==-1?wod.nSkipStart//0
		:t==-2?wod._mMediaPlayer->GetDuration()/2
		:wod._mMediaPlayer->GetDuration()-1;
	player->SetLoop(false);
	player->SetPosition(pos, true);
	bool playing = wod._isPlaying;
	if (!playing)
	{
		wod._seekbar.SetProgress(pos);
	}
	wod.PostLambda([player](){
		player->SetLoop(true);
		return false;
	}, 100);
}

void NavPlayList(int newIdx)
{
	auto & wod = XPP->_mainPlayer;
	auto & player = wod._mMediaPlayer;
	auto & lst = XPP->_playList;
	if(lst.size())
	{
		if(newIdx<0) newIdx=0;
		if(newIdx>=lst.size()) newIdx=lst.size()-1;
		//newIdx %= lst.size();
		XPP->_playIdx = newIdx;
		if(lst[newIdx]!=wod._currentPath)
		{
			wod.PlayVideoFile(lst[newIdx]);
		}
	}
}

// OnDrop
LRESULT WODApplication::HandleDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDROP hDropInfo = (HDROP)wParam;
	UINT  nFileCount = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
	TCHAR szFileName[_MAX_PATH] = TEXT("");
	DWORD dwAttribute;
	nxt_file[0] = 0;
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
			//CHAR buffer[256]={0};
			//WideCharToMultiByte (CP_ACP, 0, szFileName
			//	, -1, buffer, 256, 0, 0) ;
			//MarkPlaying();
			//LogIs(2, szFileName);
			if (!nxt_file[0])
			{
				_playList.clear();
				_playIdx = 0;
				lstrcpy(nxt_file, szFileName);
			}
			_playList.push_back(szFileName);
		}
	}
	if (nxt_file[0] && _mainPlayer._mMediaPlayer)
	{
		if (_mainPlayer.PlayVideoFile(nxt_file))
		{
			QkString filePath = nxt_file;
			PutProfString("file", "");
		}
		//_mainPlayer._mMediaPlayer->Close();
		//::KillTimer(m_hWnd, 10086);
		//::SetTimer(m_hWnd, 10086, 200, 0);
	}
	// filepath

	::DragFinish(hDropInfo);
	return true;
}


//mpv_set_option_string(mpv, "start", "00:04");

extern bool keyPressed;

void toggleFastforward(float rate) {
	if(fastForwardRate!=rate) {
		fastForwardRate = rate;
		//fastForwardStill = !XPP->_mainPlayer._isPlaying;
		if(ffTimer==0)
			ffTimer = ::SetTimer(XPP->GetHWND(), ffTimerID, ffTimerInterval, (TIMERPROC)ffTimerProc);
	} else {
		fastForwardRate = 0;
	}
}

LRESULT WODApplication::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//if(isClosing) return 0;
	switch (uMsg)
	{
	//case WM_DROPFILES:
	//{
	//	return HandleDropFiles(uMsg, wParam, lParam);
	//} break;

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
		
	} return 0;
	//case WM_PAINT: case WM_CLOSE: case WM_DESTROY:
	//case WM_SIZE: // old layout code see ::  https://github.com/KnIfER/WODPlayer/blob/9a19f3e5f0893ba82c1a8a3566e5b55f7a3e6290/src/WndControl/WODWindow.cpp#L425

	case WM_LBUTTONUP:
	{
		::ReleaseCapture();
	} return 0;

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
					PutProfString("file", "");
				}
			}
			else
				_mainPlayer.PlayVideoFile(_mainPlayer._currentPath);
		}

		//if(0)
		if ((wParam == 1 && _mainPlayer._mMediaPlayer)
			&& (_mainPlayer._mMediaPlayer->IsPlaying() || _mainPlayer._mMediaPlayer->IsPaused()) )
		{
			long pos = _mainPlayer._mMediaPlayer->GetPosition();
			long duration = _mainPlayer._mMediaPlayer->GetDuration();

			if(_mainPlayer._currentPath.EndWith("flv"))
				if (_mainPlayer._durationCache != duration)
				{
					if(duration>_mainPlayer._durationCache) _mainPlayer._durationCache = duration;
					else duration = _mainPlayer._durationCache;
				}
			int nPos =  int (pos / (double)duration * 1000);
			//_timeLabel

			int sec = pos/1000;
			int minutes = sec/60;
			int hour = minutes/60;
			minutes %= 60;
			sec %= 60;
			_timeLabel->GetText().Empty();
			_timeLabel->GetText().Format(L"%02d:%02d:%02d", hour, minutes, sec);
			//_timeLabel->GetText().Format(L"%ld", duration);
			_timeLabel->Invalidate();
			//_timeLabel->GetText() += hour;
			//_timeLabel->GetText() += L":";
			//_timeLabel->GetText() += minutes;
			//_timeLabel->GetText() += L":";
			//_timeLabel->GetText() += sec;

			sec = duration/1000;
			minutes = sec/60;
			hour = minutes/60;
			minutes %= 60;
			sec %= 60;
			_durationLabel->GetText().Empty();
			_durationLabel->GetText().Format(L"%02d:%02d:%02d", hour, minutes, sec);
			_durationLabel->Invalidate();

			if(!_mainPlayer._hPlayer) {
				_mainPlayer._hPlayer = ::GetFirstChild(_mainPlayer.GetHWND());
				_mainPlayer._mMediaPlayer->setHWND(_mainPlayer._hPlayer);
			}

			//TCHAR szPosition[64];
			//TCHAR szDuration[64];
			//g_MyPlayer.MillisecondToText(g_MyPlayer.m_nPosition, szPosition);
			//g_MyPlayer.MillisecondToText(g_MyPlayer.m_nDuration, szDuration);

			//lstrcat(szPosition, _T("/"));
			//lstrcat(szPosition, szDuration);
			// 
			//SendMessage(GetDlgItem(hwnd, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)nPos);

			if (!_mainPlayer._seekbar._isSeeking)
			{
				_mainPlayer._seekbar.SetProgressAndMax(pos, duration);
				if(_mainPlayer.nSkipStart && pos<_mainPlayer.nSkipStart-250) {
					_mainPlayer._mMediaPlayer->SetPosition(_mainPlayer.nSkipStart, true);
				}
				if(_mainPlayer.nSkipEnd && pos>=duration-_mainPlayer.nSkipEnd) {
					_mainPlayer._mMediaPlayer->SetPosition(_mainPlayer.nSkipStart, true);
				}
			}

			long sub_duration = 5*60*1000;
			if(duration<=sub_duration/5) { // 1 分钟以下
				sub_duration = 30*1000;
			}
			else if(duration<=sub_duration*2) { // 10 分钟以下
				sub_duration /= 5;
			}
			else if(duration<=sub_duration*6) { // 30 分钟以下
				sub_duration /= 2;
			}
			int range = _mainPlayer._seekfloat.GetWidth();
			int maxRange  = _mainPlayer._seekbar.GetWidth();
			sub_duration = min(sub_duration, range*1.f/2/maxRange*duration);
			if(sub_duration==0)
				sub_duration = 1000;

			_mainPlayer.seekfloat_duration = sub_duration;
			long sub_pos = pos % (sub_duration);
			int lunhui = pos / sub_duration;
			_mainPlayer._seekfloat.SetProgressAndMax(sub_pos, sub_duration);

			//_seekfloat->GetFloatPercent().left = (lunhui * sub_duration)/duration;
			//_seekfloat->NeedParentUpdate();

			int x = (int)roundf((lunhui * sub_duration)*1.f/duration*maxRange);
			if(x+range>maxRange) x = maxRange-range;
			_mainPlayer._seekfloat.SetFixedXY({x,0});

			int W = _mainPlayer._srcWidth, H=_mainPlayer._srcHeight;
			_mainPlayer._mMediaPlayer->syncResolution(_mainPlayer._srcWidth, _mainPlayer._srcHeight);
			if(_mainPlayer._srcWidth!=W || _mainPlayer._srcHeight!=H) {
				//_mainPlayer._mMediaPlayer->Pause();
				_mainPlayer.SetPos(_mainPlayer.GetPos());
			}

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
				_mainPlayer._mMediaPlayer->SetPosition(wParam, true);
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
			//case VK_OEM_COMMA:
			//	toggleFastforward(-1);
			//break;
			//case VK_OEM_PERIOD:
			//	toggleFastforward(1);
			//break;
			//case VK_OEM_2:
			//	fastForwardRate = 0;
			//break;
		}
		break;
	case WM_COPYDATA:{
		lxx(WM_COPYDATA dd dd , wParam, lParam)
		COPYDATASTRUCT *pCopyData = reinterpret_cast<COPYDATASTRUCT *>(lParam);
		if(pCopyData->dwData==WOD_COPYDATA) {
			//::SetActiveWindow(GetHWND());
			::SetForegroundWindow(GetHWND());
			//::SetFocus(GetHWND());
			wchar_t *fileNamesW = static_cast<wchar_t *>(pCopyData->lpData);
			std::vector<std::wstring> args;
			parseCommandLine(fileNamesW, args);
			lxx(ss dd, fileNamesW, args.size());
			if (args.size())
			{
				bool append = std::find(args.begin(), args.end(), L"-add")!= args.end();
				KillTimer(GetHWND(), initTimerID);
				if (hMutexTemp)
				{
					initTimer = ::SetTimer(GetHWND(), initTimerID, initTimerInterval, (TIMERPROC)initTimerProc);
				}
				for (size_t i = 0; i < args.size(); i++)
				{
					auto &  path = args[i];

					if(path.size()>0) {
						if(path[0]==L'-') {
							if(StrCmpN(args[i].c_str(), L"-loadArgs", 0)==0) {
								std::wifstream file(args[i].c_str()+10);
								if (file.is_open()) {
									std::ifstream file(args[i].c_str()+10);
									if (file.is_open()) {
										std::string line;
										QkString ln;
										while (std::getline(file, line)) {
											ln = line.c_str();
											//lxx(ss, ln.GetData())
											args.push_back(STR(ln));
										}
										file.close();
									}
								}
							}
						} 
						else {
							if (!append)
							{
								_mainPlayer.PlayVideoFile(path.c_str());
								append = true;
							}
							_playList.push_back(path.c_str());
						}
					}
				}
				
			}
		}
	} break;
	//case WM_CHAR:
	//	// 过滤非英文字符
	//	if ((wParam < 'A' || wParam > 'Z') && (wParam < 'a' || wParam > 'z')) {
	//		return 0;
	//	}
	//	break;

	//case WM_ACTIVATE:
	//	if (LOWORD(wParam) != WA_INACTIVE) {
	//		// 切换输入法为英语
	//		//HKL englishLayout = LoadKeyboardLayout(L"00000409", KLF_ACTIVATE);
	//		//ActivateKeyboardLayout(englishLayout, KLF_SETFORPROCESS);
	//		//SendMessageW(WM_INPUTLANGCHANGEREQUEST, 0, (LPARAM)LoadKeyboardLayout(L"00000409", KLF_ACTIVATE));

	//	}
	//	break;
	case WM_COMMAND:
	{
		bHandled = true;
		switch (LOWORD(wParam))
		{
		case IDM_FILE_CLOSE:
		case IDM_STOP:
			_mainPlayer.Stop();
			MarkPlaying(false);
			break;
		case IDM_FILE_OPEN:
		case IDM_OPEN:
			PickFile();
			break;
		case IDM_DELETE:
		case IDM_DELETE_FOREVER: 
		if(!keyPressed) {
			keyPressed = true;
			//lxxx(DELETE dd dd, wParam, lParam)
			_mainPlayer.Stop();
			MarkPlaying(false);
			//delPermanent = LOWORD(wParam)==IDM_DELETE_FOREVER;
			//::KillTimer(GetHWND(), delTimerID);
			//::SetTimer(GetHWND(), delTimerID, 150, (TIMERPROC)deleteFileProc);
			DeleteCurrentFile(LOWORD(wParam)==IDM_DELETE_FOREVER);
		} break;
		case IDM_PASTE_PLAYLIST: 
		case IDM_APPEND_PLAYLIST: 
		if(!keyPressed) {
			keyPressed = true;
			QkString data;
			bool purgeLst = LOWORD(wParam)==IDM_PASTE_PLAYLIST;
			if (OpenClipboard(GetHWND())) {
				HANDLE hData = GetClipboardData(CF_UNICODETEXT);
				if (hData) {
					LPWSTR pData = (LPWSTR)GlobalLock(hData);
					if (pData)
					{
						data = pData;
						GlobalUnlock(hData);
					}
				}
				CloseClipboard();
			}
			std::vector<QkString> paths;
			data.Split(L"\n", paths);
			bool detect = true;
			int idx = 0;
			for (size_t i = 0; i < paths.size(); i++)
			{
				QkString & path = paths[i];
				path.Trim();
				if(path.GetLength()==0)
					continue;
				if(path.StartWith('"') && path.EndWith('"')) {
					path.MidFast(1, path.GetLength()-2);
					path.Trim();
				}
				else if(path.StartWith(L"file")) {
					if(path.StartWith(L"file '")) {
						path.MidFast(6, path.GetLength()-7);
						path.Replace(L"'\''", L"'");
					}
					if(path.StartWith(L"file:///")) {
						path.MidFast(8, path.GetLength()-8);
					}
					path.Trim();
				}
				if(_playList.size()>0 && path.Find(L"\\")<0 && path.Find(L"/")<0) {
					if(_playList[_playList.size()-1].StartWith(L"http")) {
						_playList[_playList.size()-1].Append(L"VODNAM");
						_playList[_playList.size()-1].Append(path);
						_playList[_playList.size()-1].Append(L"NAMVOD");
					}
					continue;
				}
				if (path.GetLength()>0)
				{
					if(detect) {
						//detect = false;
						if(path.Find(L"://")>0 || path.Find(L":\\")>0) {
							detect = false;
						}
						if(!detect) {
							if(purgeLst) _playList.clear();
							idx = _playList.size();
						}
					}

					if(!detect) {
						_playList.push_back(path);
					}
				}
			}
			if(purgeLst || _mainPlayer._currentPath.IsEmpty()
				|| (!_mainPlayer._mMediaPlayer || (!_mainPlayer._mMediaPlayer->IsPaused() && !_mainPlayer._mMediaPlayer->IsPlaying()) )
					)
				NavPlayList(idx);
		} break;
		case IDM_VIEW_PROPERTY: 
		if(!keyPressed) {
			keyPressed = true;
			SHELLEXECUTEINFO info = {0};
			info.cbSize = sizeof info;
			info.lpFile = STR(_mainPlayer._currentPath);
			info.nShow = SW_SHOW;
			info.fMask = SEE_MASK_INVOKEIDLIST;
			info.fMask = SEE_MASK_INVOKEIDLIST;
			info.lpVerb = L"properties";
			ShellExecuteEx(&info);
		} break;
		case IDM_COPY_PLAYING: 
		if(!keyPressed) {
			keyPressed = true;
			const TCHAR* szText = _mainPlayer._currentPath;
			int length = _mainPlayer._currentPath.GetLength();
			if (OpenClipboard(NULL)) {
				EmptyClipboard();
				HANDLE hData = GlobalAlloc(GHND|GMEM_SHARE, (length+1)*sizeof(wchar_t));
				LPWSTR pData = (LPWSTR)GlobalLock(hData);
				CopyMemory(pData, szText, length*sizeof(wchar_t));
				pData[length] = L'\0';

				GlobalUnlock(hData);
				SetClipboardData(CF_UNICODETEXT, hData);
				CloseClipboard();
			}
		} break;
		case IDM_COPY_PLAYLIST: 
		if(!keyPressed) {
			keyPressed = true;
			int ct = _playList.size();
			std::wstring allPaths;
			for (size_t i = 0; i < ct; i++)
			{
				allPaths += _playList[i];
				if (i != ct - 1) // not the last item
				{
					allPaths += L"\r\n"; // use semicolon as separator
				}
			}
			if (allPaths.empty())
			{
				allPaths += _mainPlayer._currentPath;
			}

			// convert allPaths to char * 
			//  std::string narrowStr(allPaths.begin(), allPaths.end());
			//std::string narrowStr = WStringToString(allPaths);
			//const char* cstr = narrowStr.c_str();
			const TCHAR* szText = allPaths.c_str();
			if (OpenClipboard(NULL)) {
				EmptyClipboard();
				HANDLE hData = GlobalAlloc(GHND|GMEM_SHARE, (allPaths.size()+1)*sizeof(wchar_t));
				LPWSTR pData = (LPWSTR)GlobalLock(hData);
				CopyMemory(pData, szText, allPaths.size()*sizeof(wchar_t));
				pData[allPaths.size()] = L'\0';

				GlobalUnlock(hData);
				SetClipboardData(CF_UNICODETEXT, hData);
				CloseClipboard();
			}
		} break;
		case IDM_MIN:
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
			break;
		case IDM_MAXMISE:
			if(keyPressed) break;
			if(_isFullScreen) 
			{
				ToggleFullScreen();
				//SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); 
				break;
			}
		case IDM_MAX:
			if(keyPressed) break;
			keyPressed = 1;
			if(_isFullScreen) 
				ToggleFullScreen();
			else if(::IsMaximized(GetHWND()))
				SendMessage(WM_SYSCOMMAND, _isMaximized=SC_RESTORE, 0); 
			else
				SendMessage(WM_SYSCOMMAND, _isMaximized=SC_MAXIMIZE, 0);
			m_pm._bStaticMovable = !(_isFullScreen||_isMaximized==SC_MAXIMIZE); 
			m_pm.GetRoot()->SetInset(m_pm._bStaticMovable?5:0);
			break;
		case IDM_FULLSCREEN:
			if(keyPressed) break;
			keyPressed = 1;
			ToggleFullScreen();
			break;
		case IDM_SHUTDOWN:
			Close();
			break;
		case IDM_CLOSE:
			Close();
			break;

		case IDM_BKMK_ADD:
			//WOD_IMG_UTILS("screenshotie", _mainPlayer._mMediaPlayer->getHWND());
			_mainPlayer.AddBookmark();
			return 1;
		case IDM_BKMK_DEL:
			_mainPlayer.DelBookmark(_mainPlayer._selectedBookmark);
			return 1;
		case IDM_BKMK_RETURN:
			_mainPlayer.SelectBookMark(_mainPlayer._selectedBookmark);
			return 1;

		case IDI_PLAY:
		case IDM_TOGGLE_PLAY:
			_mainPlayer.Toggle();
			break;
		case IDM_PLAY_PAUSE:
			_mainPlayer.Toggle(0);
			break;
		case IDM_PLAY_PLAY:
			_mainPlayer.Toggle(1);
			break;

		case IDM_PLAY_TIME:
			return _mainPlayer._mMediaPlayer->GetPosition();
		case IDM_PLAY_SEEK:
			_mainPlayer._mMediaPlayer->SetPosition(lParam, HIWORD(wParam)==0);
			return 1;

		case IDM_SEEK_FORE: SeekDelta(1, 0); break;
		case IDM_SEEK_FORE_FAST: SeekDelta(1, 1); break;
		case IDM_SEEK_FORE_FASTER: SeekDelta(1, 2); break;
		case IDM_SEEK_BACK: SeekDelta(-1, 0); break;
		case IDM_SEEK_BACK_FAST: SeekDelta(-1, 1); break;
		case IDM_SEEK_BACK_FASTER: SeekDelta(-1, 2); break;


		case IDM_ROTATE_RIGHT: 
		case IDM_ROTATE_LEFT: 
			if(!keyPressed) {
				//keyPressed = true;  // 怪哉
				_mainPlayer.SetRotate(LOWORD(wParam)==IDM_ROTATE_RIGHT?90:-90); 
			}
			break;

		case IDM_SPEED_UP:  _mainPlayer.SpeedDelta(0.1f); break;
		case IDM_SPEED_DOWN:  _mainPlayer.SpeedDelta(-0.1f); break;
		case IDM_SPEED_UP_FAST:  _mainPlayer.SpeedDelta(0.5f); break;
		case IDM_SPEED_DOWN_FAST:  _mainPlayer.SpeedDelta(-0.5f); break;
		case IDM_SPEED_UP_FASTER:  _mainPlayer.SpeedDelta(1); break;
		case IDM_SPEED_DOWN_FASTER:  _mainPlayer.SpeedDelta(-1); break;
		case IDM_SPEED_RESET: _mainPlayer.SpeedDelta(0); break;

		case IDM_BKMK_PRV: NavTimemark(-1); return 1;
		case IDM_BKMK_NXT: NavTimemark(1); return 1;

		case IDM_PLAY_PRV:
		case IDM_PLAY_NXT:
			if(!keyPressed) {
				keyPressed = true;
				int newIdx = XPP->_playIdx + (LOWORD(wParam)==IDM_PLAY_PRV?-1:1);
				if(newIdx<0) newIdx=0;
				if(newIdx>=_playList.size()) newIdx=_playList.size()-1;
				NavPlayList(newIdx);
				//if (_mainPlayer._mMediaPlayer->IsPaused())
				{
					_mainPlayer._mMediaPlayer->Play();
				}
				MarkPlaying(true);
			}
			return 1;
		case IDM_PLAY_A:
		case IDM_PLAY_Z:
			if(!keyPressed) {
				keyPressed = true;
				int newIdx = (LOWORD(wParam)==IDM_PLAY_A?0:(_playList.size()-1));
				NavPlayList(newIdx);
				if (_mainPlayer._mMediaPlayer->IsPaused())
				{
					_mainPlayer._mMediaPlayer->Play();
				}
				MarkPlaying(true);
			}
			return 1;
		case IDM_PLAY_START: NavTime(-1); return 1;
		case IDM_PLAY_MID: NavTime(-2); return 1;
		case IDM_PLAY_END: NavTime(-3); return 1;

		case IDM_PLAY_FASTBACKWARD:
			toggleFastforward(-1);
			break;
		case IDM_PLAY_FASTFOREWARD:
			toggleFastforward(1);
			break;
		case IDM_PLAY_FASTFASTBACKWARD:
			toggleFastforward(-2);
			break;
		case IDM_PLAY_FASTFASTFOREWARD:
			toggleFastforward(2);
			break;
		case IDM_PLAY_FASTFASTERBACKWARD:
			toggleFastforward(-4);
			break;
		case IDM_PLAY_FASTFASTERFOREWARD:
			toggleFastforward(4);
			break;

		case IDM_PLAY_FASTFOREWARDOFF:
			fastForwardRate = 0;
			break;

		case IDM_FILE:
			trackWodMenus((CControlUI*)lParam, wParam);
			break;
		case IDM_BKMK:
			trackWodMenus((CControlUI*)lParam, wParam);
			break;
		case IDM_SKIN:
			//trackWodMenus((CControlUI*)lParam, wParam);
			// 切换hollow
			PutProfInt("WndOp", GetProfInt("WndOp", -1)!=1?1:0);
			ResetWndOpacity();
			break;
		case IDM_PLUGIN:
			//trackWodMenus((CControlUI*)lParam, wParam);
			// 切换mpv+xunlei
			if(1)
			{
				string* player = GetProfString("player");
				PutProfString("player", player && *player=="MPVExternalPlayer.dll"?"XunLeiExternalPlayer\\XunLeiExternalPlayer.dll":"MPVExternalPlayer.dll");
				
				Replay();

				auto brand = player && *player=="MPVExternalPlayer.dll"?"MVP":"XUN";
				CControlUI* menuBtn = (CControlUI*)lParam;
				menuBtn->GetText().Empty();
				menuBtn->GetText().Append(L"插件 ");
				menuBtn->GetText() += brand;
				menuBtn->SetNeedAutoCalcSize();
				menuBtn->NeedParentUpdate();
				menuBtn->NeedUpdate();
				menuBtn->Invalidate();
			}
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
		case IDM_PLUGIN_MPV:
			PutProfString("player", "MPVExternalPlayer.dll");
			Replay();
			return 1;

		case IDM_SKIN_SEEKBAR_MAGNIFIER:
			_mainPlayer._seekfloat.SetVisible(!_mainPlayer._seekfloat.IsVisible());
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
			RECT rc; GetClientRect(_mainPlayer.GetHWND(), &rc);
			::InvalidateRect(_mainPlayer.GetHWND(), 0, true);
			//::SendMessage(_mainPlayer.GetHWND(), WM_SIZE, 0, 0);
		}
		else
		{
			SetLayeredWindowAttributes(m_hWnd, TransparentKey, ceil(alpha/100.f*255), LWA_ALPHA);
		}
	}
}


void WODApplication::DeleteCurrentFile(BOOL permanent)
{
	BOOL bDelOK = 0;
	if(permanent)
	{
		extern int MoveToVacuum(PCZZSTR path);
		QkString toDel = _mainPlayer._currentPath;
		//toDel.Prepend(L"\\\\?\\");
		toDel.Append(L"\0");
		toDel.Append(L"\0");
		std::string ppp  ;
		toDel.GetData(ppp, 0);
		bDelOK = !MoveToVacuum(ppp.data());
		//bDelOK = ::DeleteFile(_mainPlayer._currentPath);
		//LogIs(2, L"del~~~ %d %s", bDelOK, _mainPlayer._currentPath);
	}
	else
	{
		//std::this_thread::sleep_for(std::chrono::microseconds(1200));
		extern int MoveToTrash(PCZZSTR path, BOOL bNoUI);
		QkString toDel = _mainPlayer._currentPath;
		//toDel.Prepend(L"\\\\?\\");
		toDel.Append(L"\0");
		toDel.Append(L"\0");
		std::string ppp  ;
		toDel.GetData(ppp, 0);
		int ret = MoveToTrash(ppp.data(), TRUE);
		//lxx(%ld , GetLastError() )
		//lxx(dd MoveToTrash, ret)
		bDelOK = !ret;
	}
	if (bDelOK)
	{
		auto & wod = XPP->_mainPlayer;
		auto & player = wod._mMediaPlayer;
		auto & lst = XPP->_playList;

		if(lst.size() && XPP->_playIdx<lst.size())
		{
			lst.erase(lst.begin()+XPP->_playIdx);
			if(XPP->_playIdx==lst.size()) 
				XPP->_playIdx --;
			if(XPP->_playIdx<0) XPP->_playIdx=0;
			if(lst.size())
			{
				wod.PlayVideoFile(lst[XPP->_playIdx]);
			}
		}
	}
}
