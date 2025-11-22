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
#include <Utils/VU.h>
#include "../resource.h"
#include "database\database_helper.h"
#include "WODMenus.h"


#include <thread>
#include <chrono>


#include "WODLrc.h"

struct DemoData
{
	const TCHAR* title;
	int image;
};

extern WODApplication* XPP;

extern int testSqlite();

extern bool hasTrack;

SeekBar* seekbar;

void makeNoTopmost(HWND hwnd) {
	if (hwnd)
	{
		::SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 100, 100, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	}
}

void makeTopmost(HWND hwnd, bool mayTop) {
	if (hwnd)
	{
		::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 100, 100, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
		if(!mayTop || !XPP || !(GetWindowLong(XPP->GetHWND(), GWL_EXSTYLE)&WS_EX_TOPMOST)) 
			makeNoTopmost(hwnd);
	}
}

WODApplication::WODApplication()
{
	_db = new WODBase();
	_frameLess = 2;
	//_frameLess = 1;
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
BOOL _singleInstance = False;

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
DWORD ffTimerInterval = 350;

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
CControlUI* _daytimeLabel;
CControlUI* _durationLabel;
BOOL _threadInit = FALSE;
ULONGLONG _lastSeekHome = 0;
ULONGLONG _lastSeekEnd = 0;

BOOL hasDayTime;
int hourDay = 0;
int minDay = 0;
int secDay = 0;


WinFrame* _osd;

void SetFloatHwndX(HWND hwnd)
{

	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

	//SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) 
	//	| WS_EX_LAYERED
	//	| WS_EX_TOOLWINDOW
	//);
	//::SetParent(hwnd, 0);
	//auto TransparentKey = RGB(0,0,0xff);
	auto TransparentKey = RGB(0x55,0x88,0x55);
	//TransparentKey = RGB(0,1,0);
	SetLayeredWindowAttributes(hwnd, TransparentKey, 0, LWA_COLORKEY);

	//RECT rc; GetClientRect(hwnd, &rc);
	//::InvalidateRect(hwnd, 0, true);

	//::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 100, 100, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
}

CControlUI* _numBtn;
Button* _selBtn;
Button* _singleBtn;
bool _selected = false;
bool _single = false;
bool _playMid = false;

long _bakTime=0;

#define blueBG  0x2e008cff
#define checkBG  0x34ffffff

void WODApplication::InitWindow()
{
	ResetWndOpacity();
	initWodMenus(this);

	m_pm._bIsLayoutOnly = true;
	_playBtn = m_pm.FindControl(_T("play"));
	_numBtn = m_pm.FindControl(_T("nump"));
	 _selBtn = static_cast<Button*>(m_pm.FindControl(_T("sel")));
	 _singleBtn = static_cast<Button*>(m_pm.FindControl(_T("sing")));
	_bottomBar = static_cast<WinFrame*>(m_pm.FindControl(_T("ch1")));
	_titleBar = static_cast<WinFrame*>(m_pm.FindControl(_T("titleBar")));
	_driveTag = static_cast<WinFrame*>(m_pm.FindControl(_T("drive")));
	_mimeTag = static_cast<WinFrame*>(m_pm.FindControl(_T("mime")));

	_topBar = static_cast<WinFrame*>(m_pm.FindControl(_T("topBar")));
	_topBarFscWnd = static_cast<WinFrame*>(m_pm.FindControl(_T("topW")));
	_topBarFscH = static_cast<WinFrame*>(m_pm.FindControl(_T("topH")));

	_timeLabel = static_cast<CControlUI*>(m_pm.FindControl(_T("time")));
	_daytimeLabel = static_cast<CControlUI*>(m_pm.FindControl(_T("daytime")));
	_durationLabel = static_cast<CControlUI*>(m_pm.FindControl(_T("duration")));

	m_pm.GetShadow()->ShowShadow(true);
	m_pm.GetShadow()->SetSize(5);
	m_pm.GetShadow()->SetSharpness(9);
	m_pm.GetShadow()->SetPosition(1,1);
	m_pm.GetShadow()->SetColor(0x55888888);

	_db->Init();

	_mainPlayer.isMain = 1;

	//m_pm.GetRoot()->PostLambda(, 250);
	string* player = GetProfString("player");
	if(player  && *player=="MPVExternalPlayer.dll") {
		_threadInit = TRUE;
		if (_playList.size()>0)
		{
			_mainPlayer._currentPath = _playList[0];
		}
		auto th = std::thread([this]{ // async
			//Sleep(750);
			auto file = GetProfString("file");
			QkString path = file?file->c_str():"";
			if (!_mainPlayer._currentPath.IsEmpty())
			{
				QkString tmp = _mainPlayer._currentPath;
				_mainPlayer._currentPath = "";
				_mainPlayer.PlayVideoFile(tmp);
			}
			//else if(!path.IsEmpty())
			//	_mainPlayer.PlayVideoFile(STR(path));
			//MarkPlaying(true);
			int cc=0;
			while(cc<100) 
			{
				cc++;
				Sleep(150);
				TimerProc();
				if(_mainPlayer._seekbar._progress>=800 && cc>3)
					break;
			}
			_threadInit = FALSE;
			return false;
		});
		th.detach();
	} else {
		auto file = GetProfString("file");
		QkString path = file?file->c_str():"";
		if (_playList.size()>0)
		{
			_mainPlayer.PlayVideoFile(_playList[0]);
		}
		else if(!path.IsEmpty())
			_mainPlayer.PlayVideoFile(STR(path));
	}

	_freeMove = GetProfInt("free_move", 0);
	_pinBottom = GetProfInt("pin_bottom", 0);

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
	//makeTopmost(hwnd, 0);
	::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 100, 100, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	::SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 100, 100, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);


	_osd = static_cast<WinFrame*>(m_pm.FindControl(_T("osd")));
	if(_osd) {
		SetFloatHwndX(_osd->GetHWND());
		_timeLabel->PostLambda([this](){
			//_osd->initRoot();
				CRichEditUI* edit = dynamic_cast<CRichEditUI*>(_osd->GetManager()->FindControl(_T("subtitle")));
				if(edit) {
					//lxx(dd, edit)
					//edit->SetFont(0, _timeLabel->GetFont());
					//edit->SetFont(L"tfont");
					edit->SetTransparent(true);
					//edit->SetRich(1);
					//edit->SetText(L"<font color=\"red\">R:\\ship\\胡萝卜</font>糊了yu - 李白乘舟将欲行，忽闻岸上牛马醒 - [24491265] - [2024-08-03] - [203101894].flv");
					//edit->SetWordCharFormat
					//edit->setbackgrou
				}
			return false;
		}, 500);

	}

	//const TCHAR* filePath = L"R:\\ship\\Godot - Using Terrain3D in Godot 4 - Part 2.srt";
	//LoadSubtitle(filePath);
	//void resetOSD();
	//resetOSD();
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
	if(_hFscBtmbar) {
		::ShowWindow(_hFscBtmbar, SW_HIDE);
		if(_topBar) ::ShowWindow(_topBar->GetHWND(), SW_HIDE);
	}
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
			//_mainPlayer._seekfloat.SetVisible(!_mainPlayer._seekfloat.IsVisible());
			::SendMessage(GetHWND(), WM_COMMAND, IDM_SKIN_SEEKBAR_MAGNIFIER_BIG, 0);
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


static DWORD dwNScStyle = WS_THICKFRAME; 
static DWORD dwNScStyle1 = WS_CAPTION|WS_THICKFRAME; 
BOOL bUseSameMiniRc = TRUE;
RECT rcNScPos;
RECT rcMiniPos;


LRESULT WINAPI FullScreenBarsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void SetFloatHwnd(HWND hwnd)
{
	auto wndSty = GetWindowLong(hwnd, GWL_STYLE);
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) 
		| WS_EX_LAYERED
		| WS_EX_TOOLWINDOW
	);
	SetParent(hwnd, 0);
	SetWindowLong(hwnd, GWL_STYLE, (wndSty  ) 
		| WS_POPUP
		| WS_SYSMENU
		//| WS_CLIPSIBLINGS 
	);
	SetWindowLong(hwnd, GWL_EXSTYLE, 0
		| WS_EX_LAYERED 
		| WS_EX_TOOLWINDOW
		//| WS_EX_TOPMOST
	);
	//SetWindowPos(hwnd, HWND_TOP , 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	makeTopmost(hwnd, 1);
}

void unsetFloatHwnd(HWND hwnd)
{
	auto wndSty = GetWindowLong(hwnd, GWL_STYLE);
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) 
		& ~WS_EX_LAYERED
		& ~WS_EX_TOOLWINDOW
	);
	SetWindowLong(hwnd, GWL_STYLE, (wndSty  ) 
		& ~WS_POPUP
		//| WS_CLIPSIBLINGS 
	);
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE)
		& ~WS_EX_LAYERED 
		& ~WS_EX_TOOLWINDOW
		//| WS_EX_TOPMOST
	);
}

void SetFloatHwnd1(HWND hwnd)
{
	//auto wndSty = GetWindowLong(hwnd, GWL_STYLE);
	//SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
	//SetWindowLong(hwnd, GWL_STYLE, (wndSty  ) 
	//	| WS_POPUP
	//	//| WS_CLIPSIBLINGS 
	//);
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE)
		| WS_EX_LAYERED 
		| WS_EX_TOOLWINDOW
		| WS_EX_TOPMOST
	);
	//SetWindowPos(hwnd, HWND_BOTTOM , 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
}

bool _lastTopMost;


void WODApplication::ToggleFullScreen1()
{
	if(!_isFullScreen && _isMini) {
		setMini();
	}
}

void WODApplication::setMini()
{
	HWND hWnd = m_hWnd;
	DWORD style = GetWindowLong(hWnd, GWL_STYLE);
	_hFscBtmbar = _bottomBar->GetHWND();
	_bottomBar->SetFloat(true);

	_topBar->GetParent()->Remove(_topBar);
	_topBarFscH->Add(_topBar);
	_topBarFscWnd->SetVisible(true);

	SetFloatHwnd(_hFscBtmbar);
	SetFloatHwnd(_topBarFscH->GetHWND());
	//SetFloatHwnd1(hWnd);

	//::SetWindowPos(_hFscBtmbar, HWND_TOPMOST, 0, 0, 100, 100, 0);
	//::SetWindowPos(_topBarFscH->GetHWND(), HWND_TOPMOST, 0, 0, 100, 100, 0);

	SetLayeredWindowAttributes(_hFscBtmbar, TransparentKey, 200, LWA_ALPHA);

	GetWindowRect(hWnd, &rcNScPos);

	/* POPUP 可以覆盖任务栏 */
	//SetWindowLong(hWnd, GWL_STYLE , style & ~dwNScStyle  | WS_POPUP );
	//SetWindowLong(hWnd, GWL_STYLE , style & ~WS_THICKFRAME | WS_POPUP | WS_CAPTION );

	//style = GetWindowLong(hWnd, GWL_EXSTYLE);
	//dwNScStyle = WS_EX_DLGMODALFRAME |
	//	WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE;
	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_TOPMOST);
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

	_topBarFscWnd->SetVisible(false);
	_bottomBar->SetVisible(false);

	_bottomBar->SetFloat(true);
	m_pm.GetSizeBox().top = 0;

	//int w = ::GetSystemMetrics(SM_CXSCREEN)/2;
	//int h = ::GetSystemMetrics(SM_CYSCREEN);
	////_lastTopMost = GetWindowLong(hWnd, GWL_EXSTYLE)&WS_EX_TOPMOST;
	//::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, w, h, 0);
	resetInset();
}

void WODApplication::ToggleMini()
{
	HWND hWnd = m_hWnd;
	DWORD style = GetWindowLong(hWnd, GWL_STYLE);
	if (!_isMini)
	{
		_isMini = true;
		setMini();
		return;
	}
	else
	{
		_bottomBar->SetFloat(false);
		SetParent(_mainPlayer._seekbar.GetHWND(), m_hWnd);
		_hFscBtmbar = nullptr;
		_isMini = false;
		GetWindowRect(hWnd, &rcMiniPos);
		style = GetWindowLong(hWnd, GWL_STYLE);
		SetWindowLong(hWnd, GWL_STYLE , style | WS_THICKFRAME );

		if(bUseSameMiniRc) {
			GetWindowRect(hWnd, &rcNScPos);
		}
		::SetWindowPos(hWnd, NULL, rcNScPos.left, rcNScPos.top
			, rcNScPos.right-rcNScPos.left, rcNScPos.bottom-rcNScPos.top, 0);

		_topBar->GetParent()->Remove(_topBar);
		m_pm.GetRoot()->AddAt(_topBar, 0);
		_bottomBar->SetVisible(true);
		_topBarFscWnd->SetVisible(false);
		m_pm.GetSizeBox().top = 4;
		unsetFloatHwnd(_bottomBar->GetHWND());
		//_bottomBar->PostLambda([this](){
		//	_bottomBar->Invalidate();
		//	return false;
		//}
		//, 1000);
		if(_lastTopMost) {
			//SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE)|WS_EX_TOPMOST);
			//::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		}

		UINT style = GetWindowLong(GetHWND(), GWL_STYLE);
		SetWindowLong(GetHWND(), GWL_STYLE, style & ~WS_POPUP | WS_THICKFRAME);
		SetWindowLong(GetHWND(), GWL_EXSTYLE, GetWindowLong(GetHWND(), GWL_EXSTYLE) & ~WS_EX_TOPMOST);
		::SetWindowPos(GetHWND(), HWND_NOTOPMOST, 0, 0, 100, 100, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
	}
	resetInset();
	/* 配合 POPUP 使用 */
	//_frameLess = 1;

}


void resetOSD()
{
	if(_osd) {
		bool vis = subsCnt>0;
		_osd->SetVisible(vis);
	}
}

void WODApplication::resetInset()
{
	//m_pm.GetRoot()->SetInset(0);

	//bool b1 = _isMini||_isMaximized==SC_MAXIMIZE;
	//m_pm.GetRoot()->SetInset(b1?1:5);

	int val = 1;

	if(::IsMaximized(GetHWND())||_isFullScreen)
		val = 0;


	m_pm.GetRoot()->SetInset(val);


	if(_osd) {
		_osd->SupressChildLayouts(1);
		int nPad = _isFullScreen||_isMini?_bottomBar->GetHeight():25;
		CControlUI* pad = dynamic_cast<CControlUI*>(_osd->GetManager()->FindControl(_T("bpad")));
		if(pad) {
			pad->SetFixedHeight(nPad);
		}
		//_osd->GetInnerView()->GetItemAt(0)->NeedUpdate();
		InputBox* edit = dynamic_cast<InputBox*>(_osd->GetManager()->FindControl(_T("subtitle")));
		if(edit) {
			edit->PostLambda([edit](){
				edit->NeedParentAutoUpdate();
				return false;
			}, 250);
		}
		_osd->SupressChildLayouts(0);
	}
}

void WODApplication::ToggleFullScreen()
{
	HWND hWnd = m_hWnd;
	DWORD style = GetWindowLong(hWnd, GWL_STYLE);
	_frameLess = !_isFullScreen?1:2;
	DWORD dwNCSty = dwNScStyle;
	if(/*_isMini && */(::IsMaximized(GetHWND())))
		dwNCSty = dwNScStyle1;
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


		//SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_TOPMOST);
		//SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

		style = GetWindowLong(hWnd, GWL_STYLE);
		SetWindowLong(hWnd, GWL_STYLE , (style & ~dwNCSty) | WS_POPUP );

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
		_lastTopMost = GetWindowLong(hWnd, GWL_EXSTYLE)&WS_EX_TOPMOST;
		::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, w, h, 0);

		//HMONITOR hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
		//MONITORINFO mi = { sizeof(mi) };
		//if (GetMonitorInfo(hmon, &mi))
		//{
		//	::SetWindowPos(hWnd, HWND_TOPMOST, mi.rcMonitor.left,
		//		mi.rcMonitor.top,
		//		mi.rcMonitor.right - mi.rcMonitor.left,
		//		mi.rcMonitor.bottom - mi.rcMonitor.top, 0);
		//}

		//if(GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST) {
		//SetWindowLong(_hFullScreenBtmbar, GWL_EXSTYLE, GetWindowLong(_hFullScreenBtmbar, GWL_EXSTYLE)|WS_EX_TOPMOST);
		//SetWindowLong(this->_topBarFscH->GetHWND(), GWL_EXSTYLE, GetWindowLong(this->_topBarFscH->GetHWND(), GWL_EXSTYLE)|WS_EX_TOPMOST);
		//}

		_bottomBar->PostLambda([_hFullScreenBtmbar, hWnd, this](){
			if(!this->_isFullScreen) 
				return false;
			SetWindowPos(_hFullScreenBtmbar, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
			//::SetActiveWindow(hWnd);
			::SetForegroundWindow(hWnd);
			if(_lastTopMost) {
				SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE)&~WS_EX_TOPMOST);
				::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
			}
			return false;
		}
		, 100);
	}
	else
	{
		_bottomBar->SetFloat(false);
		::SetParent(_mainPlayer._seekbar.GetHWND(), m_hWnd);
		_isFullScreen = false;
		style = GetWindowLong(hWnd, GWL_STYLE);
		SetWindowLong(hWnd, GWL_STYLE , style | dwNCSty );

		::SetWindowPos(hWnd, NULL, rcNScPos.left, rcNScPos.top
			, rcNScPos.right-rcNScPos.left, rcNScPos.bottom-rcNScPos.top, 0);

		_topBar->GetParent()->Remove(_topBar);
		m_pm.GetRoot()->AddAt(_topBar, 0);
		_bottomBar->SetVisible(true);
		_topBarFscWnd->SetVisible(false);
		m_pm.GetSizeBox().top = 4;
		if(_lastTopMost) {
			SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE)|WS_EX_TOPMOST);
			::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		}


		//_bottomBar->SetVisible(false);
		//_bottomBar->SetFloat(true);
		//_bottomBar->SetFloat(false);
		//_bottomBar->SetVisible(true);

		if(!_isMini)
		if(::GetParent(_bottomBar->GetHWND())!=m_hWnd) { // 退出全屏后。重建底栏，因为有时分离
			//lxx(wtf)
			//::SetParent(_bottomBar->GetHWND(), m_hWnd);
			


			auto old = _bottomBar;
			_bottomBar = new WinFrame();
			auto inner = old->GetInnerView();
			_bottomBar->Add(inner);
			_bottomBar->SetFloatAlign(DT_BOTTOM);
			old->GetParent()->AddAt(_bottomBar, old->GetParent()->GetItemIndex(old));
			old->GetParent()->Remove(old);
			_bottomBar->Init();
			_bottomBar->SetVisible(true);


			_bottomBar->SetFixedHeight(-2);
			_bottomBar->SetFixedWidth(-1);
			//_bottomBar->Add(inner);

			//lxx(dd dd dd dd, ::GetParent(_bottomBar->GetHWND())==m_hWnd, _bottomBar->GetHWND(), _bottomBar->GetCount(), inner);


			//::ShowWindow(old->GetHWND(), SW_HIDE);
			::DestroyWindow(old->GetHWND());
			//auto inner = old->GetItemAt(0);
			//old->Remove(inner);
			//m_pm.GetRoot()->Add(inner);
			delete old;
		}

	}
	resetInset();
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
	if(playing && hMutexReady) { // 释放互斥体
		ReleaseMutex(hMutexReady);
		CloseHandle(hMutexReady);
		hMutexReady = 0;
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

void player_SetPosition(VideoPlayer* player, long pos)
{
	player->SetPosition(pos, true);
	if(hasTrack) 
	{
		XPP->_audioPlayer.SetPosition(pos, true);
	}
}

void SeekDelta(int delta, int level)
{
	auto & player = XPP->_mainPlayer._mMediaPlayer;
	if (player)
	{
		fastForwardRate = 0;
		long dur=XPP->_mainPlayer.GetDuration();
		//lzzx(%ld %ld , dur, XPP->_mainPlayer._seekbar._max);
		//long dur=XPP->_mainPlayer._seekbar._max;
		float factor = 1;
		if(level==1) {
			factor = 5;
		}
		if(level==2) {
			factor = 30;
		}
		//lzz(set = dd ff ff , delta, delta*factor*1000, 1.f);
		long res = player->GetPosition()+delta*factor*1000;
		bool set=0;
		if(res<0)  res=0;
		else if(res>dur) res=dur;
		else set=1;
		if(set || player->GetPosition()!=res) {
			player_SetPosition(player, res);
		}
		if(!XPP->_mainPlayer._isPlaying && !XPP->_mainPlayer._seekbar._isSeeking) {
			XPP->_mainPlayer._seekbar.SetProgressAndMax(res, XPP->_mainPlayer.GetDuration());
		}
	}
}

void SeekEx(int cmd)
{
	auto & player = XPP->_mainPlayer._mMediaPlayer;
	if (player)
	{
		player->SetPositionEx(cmd, (LONG)NULL);
		if(cmd<3 && _playing) XPP->_mainPlayer.Toggle(0);
		if (XPP->_mainPlayer.isMain && hasTrack)
		{
			XPP->_audioPlayer.SetPosition(player->GetPosition(), true);
			if (cmd < 3 && _playing) XPP->_audioPlayer.Toggle(0);
		}
	}
}

int NavTimemark(int delta)
{
	auto & wod = XPP->_mainPlayer;
	if(!wod._mMediaPlayer) return -1;
	auto & player = wod._mMediaPlayer;
	auto & bkmks = wod._bookmarks;
	int pos = player->IsPlaying()?wod._seekbar._progress:player->GetPosition(); // todo
	_bakTime = pos;
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
		return wod._selectedBookmark;
	}
}

//std::function<bool (void)> haha = [](){return 0;};

void NavTime(int t)
{
	auto & wod = XPP->_mainPlayer;
	if(!wod._mMediaPlayer) return;
	auto & player = wod._mMediaPlayer;
	LONG pos = t==-1?wod.nSkipStart//0
		:t==-2?wod.GetDuration()/2
		:wod.GetDuration()-1;
	player->SetLoop(false);
	player_SetPosition(player, pos);
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

set<QkString> selMap; // of paths

void setSelIndi() {
	bool selected = selMap.find(XPP->_mainPlayer._currentPath)!=selMap.end();
	if(_selected!=selected) {
		_selBtn->SetText(selected?L"已选中":L"未选中");
		_selBtn->SetTextColor(selected?0xffffffff:0xffbbbbbb); // 0xffff5555
		_selBtn->SetBkColor(selected?checkBG:0); // 0x34ffffff 0xff333333
		_selBtn->SetHotBkColor(selected?checkBG:0x34ffffff); // 0x34ffffff 0xff333333
		_selected = selected;
	}
}

void parseTimeTag(QkString input) {
	// Find the positions of the brackets
	hourDay = 0;
	minDay = 0;
	secDay = 0;
	int first = input.Find(L"监控");
	if (first < 0) {
		_daytimeLabel->GetText().Empty();
		hasDayTime = false;
		return;
	}
	int firstBracket = input.Find(L'[', first);
	int secondBracket = firstBracket==-1?-1:input.Find(L']', firstBracket);
	int thirdBracket = secondBracket == -1 ? -1 : input.Find(L'[', secondBracket);
	int fourthBracket = thirdBracket == -1 ? -1 : input.Find(L']', thirdBracket);

	if (firstBracket != -1 && secondBracket != -1) {
		// Extract the date and time strings
		QkString dateStr = input.Mid(firstBracket + 1, secondBracket - firstBracket - 1);
		//QkString timeStr = input.Mid(thirdBracket + 1, fourthBracket - thirdBracket - 1);

		//&& thirdBracket != -1 && fourthBracket != -1
		// Parse the date components
		
		if (dateStr.GetLength()>=10) { // 2025-01-15 17:11:09
			int year = ParseInt(dateStr.Mid(0, 4));
			int month = ParseInt(dateStr.Mid(5, 2));
			int day = ParseInt(dateStr.Mid(8, 2));
			if (dateStr.GetLength() >= 17) {
				dateStr.MidFast(10);
				dateStr.Trim();
				if (dateStr.GetLength() >= 6) { // 171109
					hasDayTime = true;
					hourDay = ParseInt(dateStr.Mid(0, 2));
					minDay = ParseInt(dateStr.Mid(2, 2));
					secDay = ParseInt(dateStr.Mid(4, 2));
				}
			}
		}


		//int millisecond = timeStr.length() > 6 ? timeStr.mid(6, 2).toInt() : 0;

		//int hour = timeStr.Mid(0, 2).toInt();
		//int minute = timeStr.Mid(2, 2).toInt();
		//int second = timeStr.Mid(4, 2).toInt();

	}
	else {

	}
}

extern int py_main(QkString path);

void WODApplication::onNewVideo()
{
	auto & wod = XPP->_mainPlayer;
	auto & player = wod._mMediaPlayer;
	auto & lst = XPP->_playList;

	const auto & current = wod._currentPath;


	subsCnt = 0;
	resetOSD();

	auto tm = GetTickCount();
	py_main(current);
	lxx(time::dd, GetTickCount() - tm);

	player->Command(2, !subsCnt);
	//player->Command(2, 1);


	if(current.EndWith(L"video.mp4")) {
		QkString path = current;
		path.ReplaceEx(L"video.mp4", L"audio.flac");
		XPP->_audioPlayer.PlayVideoFile(path);
		//pzz(STR(path))
	}

	parseTimeTag(_titleBar->GetText());

	if(!_numBtn) 
		return;
	if(GetProfInt("mid", 0)) 
		_playMid = true;
	auto & text = _numBtn->GetText();
	text.AsBuffer();
	text.FormatEx(L"%d / %d", false, XPP->_playIdx+1, XPP->_playList.size());
	_numBtn->SetText(text);
	//_numBtn->SetFixedWidth(-2);
	_numBtn->NeedParentUpdate();

	// todo opt
	setSelIndi();
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

void NextPlayList(bool prev, bool loop)
{
	auto & wod = XPP->_mainPlayer;
	auto & player = wod._mMediaPlayer;
	auto & lst = XPP->_playList;

	int newIdx = XPP->_playIdx + (prev?-1:1);
	if(newIdx<0) newIdx=0;
	if(newIdx>=lst.size()) newIdx=loop?0:(lst.size()-1);
	NavPlayList(newIdx);
	//if (_mainPlayer._mMediaPlayer->IsPaused())
	{
		player->Play();
	}
	XPP->MarkPlaying(true);
}


void NextPlayGroup(bool prev, bool loop)
{
	auto & wod = XPP->_mainPlayer;
	auto & player = wod._mMediaPlayer;
	auto & lst = XPP->_playList;

	int d = (prev?-1:1);
	int newIdx = XPP->_playIdx;
	QkString prefix = wod._currentPath;
	int tmp = prefix.Find(L" - ");
	if(tmp>0) {
		prefix.MidFast(0, tmp);
		QkString pfx1;
		while(1) {
			newIdx += d;
			if(newIdx<0 || newIdx>=lst.size()) {
				if(newIdx<0) newIdx=lst.size()-1;
				else newIdx=0;
				break;
			}
			pfx1 = lst[newIdx];
			tmp = pfx1.Find(L" - ");
			if(tmp>0) {
				pfx1.MidFast(0, tmp);
				if(pfx1!=prefix) {
					break;
				}
			}
		}
		NavPlayList(newIdx);
		//if (_mainPlayer._mMediaPlayer->IsPaused())
		{
			player->Play();
		}
		XPP->MarkPlaying(true);
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
			QkString path_ = szFileName;
			if(path_.EndWith(L".srt")) {
				LoadSubtitle(path_);
				//lxxz(dd, subsCnt);
				resetOSD();
			}
			else if(!path_.EndWith(L".txt")) { // todo opt
				//LogIs(2, STR(path_));
				if (!nxt_file[0])
				{
					_playList.clear();
					_playIdx = 0;
					lstrcpy(nxt_file, szFileName);
				}
				_playList.push_back(szFileName);
			}
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


int lastLun = 0;

LRESULT WODApplication::TimerProc()  // ontiemchange
{
	//if(_osd) {
	//	makeTopmost(_osd->GetHWND(), 0);
	//}
	if(!_mainPlayer._mMediaPlayer)
		return 0;
	long pos = _mainPlayer.GetPosition(true);
	long duration = _mainPlayer.GetDuration();

	if(_mainPlayer._currentPath.EndWith("flv"))
		if (_mainPlayer._durationCache != duration)
		{
			if(duration>_mainPlayer._durationCache) _mainPlayer._durationCache = duration;
			else duration = _mainPlayer._durationCache;
		}
	int nPos =  int (pos / (double)duration * 1000);
	//_timeLabel
	SyncSubtitles(pos);

	int sec = pos/1000;
	int minutes = sec/60;
	int hour = minutes/60;
	minutes %= 60;
	sec %= 60;
	_timeLabel->GetText().FormatEx(L"%02d:%02d:%02d", false, hour, minutes, sec);
	//_timeLabel->GetText().Format(L"%ld", duration);
	_timeLabel->Invalidate();
	//_timeLabel->GetText() += hour;
	//_timeLabel->GetText() += L":";
	//_timeLabel->GetText() += minutes;
	//_timeLabel->GetText() += L":";
	//_timeLabel->GetText() += sec;

	sec += secDay; if (sec > 60) { sec -= 60; minutes++; }
	minutes += minDay; if (minutes > 60) { minutes -= 60; hour++; }
	hour += hourDay;

	if (hasDayTime) {
		_daytimeLabel->GetText().FormatEx(L"%02d:%02d:%02d", false, hour, minutes, sec);
		//_daytimeLabel->GetText().Format(L"%ld", duration);
		_daytimeLabel->Invalidate();
	}

	sec = duration/1000;
	minutes = sec/60;
	hour = minutes/60;
	minutes %= 60;
	sec %= 60;
	_durationLabel->GetText().Empty();
	_durationLabel->GetText().Format(L"%02d:%02d:%02d", hour, minutes, sec);
	_durationLabel->Invalidate();

	if(!IsWindow(_mainPlayer._hPlayer)) { // todo opt
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
			_mainPlayer.SetPosition(_mainPlayer.nSkipStart, true);
		}
		if(_mainPlayer.nSkipEnd && pos>=duration-_mainPlayer.nSkipEnd) {
			_mainPlayer.SetPosition(_mainPlayer.nSkipStart, true);
		}
	}
	if(!deleting && !_single) 
	{
		if (_playList.size()>1)
		{
			if(pos >= duration-350 && _mainPlayer._isPlaying) 
			{
				if(!_mainPlayer._mMediaPlayer->IsPlaying()) 
				{
					if (_playList.size()>1)
					{
						NextPlayList(0, 1);
					}
					return 0;
				}
				if (_playList.size()>1)
				{
					//NextPlayList(0, 1);
					_mainPlayer._mMediaPlayer->SetLoop(false);
				}
				//return 0;
			}
			if(pos >= duration-10 && _mainPlayer._isPlaying) 
			{
				if (_playList.size()>1)
				{
					NextPlayList(0, 1);
				}
				//return 0;
			}
		}
	} else {
		_mainPlayer._mMediaPlayer->SetLoop(1);
	}
	if(_playMid && _mainPlayer._seekbar._progress<_mainPlayer._seekbar._max/8) 
	{
		_mainPlayer.SetPosition(_mainPlayer.GetDuration()/2, true);
		_playMid = false;
	}

	int mag_width = min(_mainPlayer._seekbar.GetWidth(), 350);

	long sub_duration = 5*60*1000;
	int range = mag_width;
	int maxRange = _mainPlayer._seekbar.GetWidth();
	if (_mag_type == 0) {
		if (duration <= sub_duration / 5) { // 1 分钟以下
			sub_duration = 30 * 1000;
		}
		else if (duration <= sub_duration * 2) { // 10 分钟以下
			sub_duration /= 5;
		}
		else if (duration <= sub_duration * 6) { // 30 分钟以下
			sub_duration /= 2;
		}
		sub_duration = min(sub_duration, range * 1.f / 2 / maxRange * duration);
	}
	else {
		mag_width = _mainPlayer._seekbar.GetWidth();
		sub_duration = min(1.25 * 60 * 60 * 1000, duration);
		sub_duration = min(30 * 60 * 1000, duration);
		if (duration <= sub_duration + 5 * 60 * 1000) {
			sub_duration /= 2;
		}
	}
	if (_mainPlayer._seekfloat.GetWidth() != mag_width)
		_mainPlayer._seekfloat.SetFixedWidth(mag_width);

	if(sub_duration==0)
		sub_duration = 1000;

	_mainPlayer.seekfloat_duration = sub_duration;
	long sub_pos = pos % (sub_duration);
	int lunhui = pos / sub_duration;
	_mainPlayer._seekfloat.SetProgressAndMax(sub_pos, sub_duration);

	//_seekfloat->GetFloatPercent().left = (lunhui * sub_duration)/duration;
	//_seekfloat->NeedParentUpdate();

	int x = (int)roundf((lunhui * sub_duration)*1.f/duration*maxRange);
	if (lastLun != x) {
		//_mainPlayer._seekfloat._isSeeking = false;
		_mainPlayer._seekfloat.dragDownLeft = _mainPlayer._seekfloat.dragDownX;
		lastLun = x;
	}
	if(x+range>maxRange) x = maxRange-range;
	if (x + _mainPlayer._seekfloat.GetWidth() > _mainPlayer._seekfloat.GetParent()->GetWidth()) {
		x = _mainPlayer._seekfloat.GetParent()->GetWidth() - _mainPlayer._seekfloat.GetWidth();
	}
	//int mag_width = min(_mainPlayer._seekbar.GetWidth(), 550);
	//mag_width = _mainPlayer._seekbar.GetWidth();
	//if(_mainPlayer._seekfloat.GetWidth()!= mag_width)
	//	_mainPlayer._seekfloat.SetFixedWidth(mag_width);
	if (x < 0)
		x = 0;
	//lxx(dd dd dd dd, x + _mainPlayer._seekfloat.GetWidth(), x, _mainPlayer._seekfloat.GetWidth(), _mainPlayer._seekfloat.GetParent()->GetWidth())

	_mainPlayer._seekfloat.SetFixedXY({x,0});

	int W = _mainPlayer._srcWidth, H=_mainPlayer._srcHeight;
	_mainPlayer._mMediaPlayer->syncResolution(_mainPlayer._srcWidth, _mainPlayer._srcHeight);
	if(_mainPlayer._srcWidth!=W || _mainPlayer._srcHeight!=H) {
		//_mainPlayer._mMediaPlayer->Pause();
		_mainPlayer.SetPos(_mainPlayer.GetPos());
	}

	//LogIs(3, "setPosition:: %d %d max=%d curr=%d\n", _mainPlayer._mMediaPlayer->m_nPosition, _mainPlayer._mMediaPlayer->m_nDuration, _mainPlayer._seekbar.GetMax(), _mainPlayer._seekbar.GetPosition());
	return 0;
}

extern  BOOL iconized;
extern  BOOL paused;
BOOL resumePlay = false;

void WODApplication::onPause(bool min) 
{
	paused = true;
	if(min) {
		iconized = 1;
		if(_hFscBtmbar && _bottomBar->IsVisible() && !_pinBottom) {
			_bottomBar->SetVisible(false);
			//lxx(1)
			//makeNoTopmost(_hFscBtmbar);

			//_bottomBar->PostLambda([this](){
			//	ShowWindow(GetHWND(), SW_MINIMIZE);
			//	return false;
			//}, 800);
		}

		if(_topBarFscWnd && _topBarFscWnd->IsVisible()) {
			_topBarFscWnd->SetVisible(false);
		}
		//::ShowWindow(_hFscBtmbar, SW_HIDE);
		//lxx(hide)
		if(resumePlay=_mainPlayer._isPlaying)
			_mainPlayer.Toggle();
		//_mainPlayer._mMediaPlayer->Command(20250910,1); // music
	}
	m_pm.GetRoot()->SetBkColor(0xff555555);
}

void WODApplication::onResume(bool min) {
	paused = false;
	if(_bottomBar && _bottomBar->IsVisible()) {
		if(min || iconized) {
			if(_hFscBtmbar && _bottomBar->IsVisible()) {
				::ShowWindow(_hFscBtmbar, SW_SHOWNOACTIVATE);
			}
		}
		makeTopmost(_hFscBtmbar, 1);
	}
	if(min || iconized) {
		if(resumePlay)
			_mainPlayer.Toggle(true);
		//_mainPlayer._mMediaPlayer->Command(20250910, 0);
		resumePlay = false;
	}
	iconized = 0;
	auto bk = 0xFF0078d7;
	m_pm.GetRoot()->SetBkColor(bk);

	//makeTopmost(GetHWND(),0);

	//if (IsIconic(GetHWND()))
	//	::SendMessage(GetHWND(), WM_SYSCOMMAND, SC_RESTORE, 0);
	//VU::SetForegroundWindowEx(GetHWND());
	//SetForegroundWindow(GetHWND()); // 会导致mini底栏首次聚焦无法点击
}

void AdjustVoice() {
	extern int volume;
	XPP->_mainPlayer.SetVolume(XPP->_muteL ? 0 : volume, XPP->_muteR ? 0 : volume);

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

	case WM_STYLECHANGED:
	{
		//if(wParam==GWL_STYLE) {
		//	LPSTYLESTRUCT lpSty = (LPSTYLESTRUCT)lParam;
		//	if((lpSty->styleNew&WS_POPUP) != (lpSty->styleOld&WS_POPUP)) {
		//		_frameLess = (lpSty->styleNew&WS_POPUP)?1:2;
		//	}
		//}
	} break;
	case WM_SETFOCUS:
	{
		LogIs(L"WM_SETFOCUS");
		//if(_osd) {
		//	_osd->PostLambda([](){
		//		//makeNoTopmost(_osd->GetHWND());
		//		makeTopmost(_osd->GetHWND(), 0);
		//		return false;
		//	}, 10);
		//}
		if (_pinBottom && _hFscBtmbar && !_isFullScreen)
			_mainPlayer.PostLambda([this]()
			{
				_bottomBar->SetVisible(1);
				if (_hFscBtmbar) makeTopmost(_hFscBtmbar, 1);
				return false;
			}, 50);
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
	case WM_SIZE: // old layout code see ::  https://github.com/KnIfER/WODPlayer/blob/9a19f3e5f0893ba82c1a8a3566e5b55f7a3e6290/src/WndControl/WODWindow.cpp#L425
		if (wParam == SIZE_MINIMIZED) {
			onPause(1);
		}
		if (wParam == SIZE_MAXIMIZED) {
			resetInset();
		}
		if (wParam == SIZE_RESTORED) {
			resetInset();
		}
	return 0;
	case WM_MOVE:
	{
		if(_osd) { // _osd
			RECT rc = _osd->GetPos();
			RECT pRc;

			GetWindowRect(_osd->GetManager()->GetRealManager()->GetPaintWindow(), &pRc);
			::OffsetRect(&rc, pRc.left, pRc.top);

			SetWindowPos(_osd->GetHWND(), 0, rc.left, rc.top, 0, 0, SWP_NOSIZE);
		}
		if (_pinBottom && _hFscBtmbar && !_isFullScreen) {
			//_bottomBar->GetParent()->NeedUpdate();
			//_bottomBar->SetVisible(true);
			//_bottomBar->ParentNeedUpdate();


			RECT rc = _bottomBar->GetPos();
			RECT pRc;

			GetWindowRect(_osd->GetManager()->GetRealManager()->GetPaintWindow(), &pRc);
			::OffsetRect(&rc, pRc.left, pRc.top);

			SetWindowPos(_hFscBtmbar, HWND_TOPMOST, rc.left, rc.top, 0, 0, SWP_NOSIZE);

			//_mainPlayer.PostLambda([this]() {
			//	_bottomBar->SetVisible(1);
			//	if(_hFscBtmbar) makeTopmost(_hFscBtmbar, 1);
			//	return false;
			//}, 250);
		}
	} return 0;
	
	case WM_LBUTTONUP:
	{
		::ReleaseCapture();
	} return 0;

	case WM_TIMER: // sese
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
		if(!_threadInit)
		if ((wParam == 1 && _mainPlayer._mMediaPlayer)
			&& (_mainPlayer._mMediaPlayer->IsPlaying() || _mainPlayer._mMediaPlayer->IsPaused() || _mainPlayer.isPng && _playing) )
		{
			return TimerProc();
		}
	}
	return 0;

	case WM_SYSCOMMAND:
	{
		if (wParam == SC_MINIMIZE)
		{
			//lxx(min)
			//onPause(1);
		}
		if (wParam == SC_RESTORE)
		{
			//LogIs(2, "onResume");
			onResume(1);
		}
	} break;
	case WM_ACTIVATEAPP:
	{
		if (wParam == TRUE) // if npp is about to be activated
		{
			onResume();
		}
		else
		{
			onPause();
		}
		return FALSE;
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
				_mainPlayer.SetPosition(wParam, true);
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
		//lxx(WM_COPYDATA dd dd , wParam, lParam)
		//_bottomBar->PostLambda([](){
		//	static int findCC = 0;
		//	findCC++;
		//	PathFinder _pathFinder;
		//	std::vector<QkString> fileArr;
		//	set<QkString, strPtrCmp>  parents;
		//	_pathFinder.SetParentRecorder(parents);
		//	_pathFinder.GetPatterns().push_back(L"*");
		//	_pathFinder.SearchFiles((findCC%2)?L"V:\\BaiduNetdiskDownload":L"F:\\Backup", &fileArr, 0, true);
		//	lxxx(find :: dd, fileArr.size())
		//	return true;
		//}, 5000);
		COPYDATASTRUCT *pCopyData = reinterpret_cast<COPYDATASTRUCT *>(lParam);
		if(pCopyData->dwData==WOD_COPYDATA) {
			KillTimer(GetHWND(), initTimerID);
			if (IsIconic(GetHWND()))
				::SendMessage(GetHWND(), WM_SYSCOMMAND, SC_RESTORE, 0);
				//teSetForegroundWindow(GetHWND());
			 //::SetActiveWindow(GetHWND());
			//if (IsIconic(GetHWND()))
				//ShowWindow(GetHWND(), SW_RESTORE);
			auto bSetFore = ::SetForegroundWindow(GetHWND());
			//;
			//::SetFocus(GetHWND());
			wchar_t *fileNamesW = static_cast<wchar_t *>(pCopyData->lpData);
			//fileNamesW[pCopyData->dwData] = 0;
			std::vector<std::wstring> args;
			parseCommandLine(fileNamesW, args);
			//lxxz(ss dd, fileNamesW, args.size());
			bool append = std::find(args.begin(), args.end(), L"-add")!= args.end();
			if(!append) {
				selMap.clear();
				XPP->_mainPlayer.Stop();
			}
			bool stopped = XPP->_mainPlayer.bStopped;
			if (args.size())
			{
				if(append)
					stopped = False;
				if(stopped)
					XPP->_playList.clear();
				if(!hMutexTemp)
					hMutexTemp = CreateMutex(NULL, TRUE, L"WODPLTMP1");
				if (hMutexTemp)
				{
					initTimer = ::SetTimer(GetHWND(), initTimerID, initTimerInterval, (TIMERPROC)initTimerProc);
				}
				extern void parseArgs(std::vector<std::wstring> & args) ;
				parseArgs(args);
				if(stopped) {
					XPP->_mainPlayer._currentPath.Empty();
					NavPlayList(0);
					XPP->_mainPlayer.Toggle(1);
				}
			}
			if(!bSetFore) {
				VU::SetForegroundWindowEx(GetHWND());
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
		HWND hwnd = GetHWND();
		UINT style;
		bHandled = true;
		switch (LOWORD(wParam))
		{
		case IDM_FILE_CLOSE:
		case IDM_STOP:
			_mainPlayer.Stop();
			MarkPlaying(false);
			if(!hMutexReady)
				hMutexReady = CreateMutex(NULL, TRUE, L"WODPLTRDY");
			//QkString readyT = _titleBar->GetText();
			::SetWindowText(GetHWND(), readyT);
			break;
		case IDM_FILE_OPEN:
		case IDM_OPEN:
			PickFile();
			break;
		case IDM_NO_DELELTE_TOGGLE:
			_safe_mode = !_safe_mode;
			closeMenus(true);
			break;
		case IDM_BKMK_MUTE_TRACK:
			if (_bkmk_add_layer == 2) _bkmk_add_layer = 0;
			else _bkmk_add_layer = 2;
			closeMenus(true);
			break;
		case IDM_BKMK_SEP_TRACK:
			if (_bkmk_add_layer == 1) _bkmk_add_layer = 0;
			else _bkmk_add_layer = 1;
			closeMenus(true);
			break;
		case IDM_AUDIO_OPEN:
			PickAudio();
			break;
		case IDM_AUDIO_RELOAD: // reload track
			//PickAudio();
			if(hasTrack) {
				_audioPlayer.Stop();
				if(_audioPlayer.PlayVideoFile(_audioPlayer._currentPath))
					_audioPlayer.SetPosition(_mainPlayer.GetPosition(false), true);
			}
			break;
		case IDM_AUDIO_COPY:
			//PickAudio();
			if(hasTrack) {
				SetTmpText(_audioPlayer._currentPath, _audioPlayer._currentPath.GetLength());
			}
			break;
		case IDM_DELETE:
		case IDM_DELETE_FOREVER: 
		if(!keyPressed) {
			//keyPressed = true;
			deleting = TRUE;
			//lxxx(DELETE dd dd, wParam, lParam)
			string* player = GetProfString("player");
			if(*player!="MPVExternalPlayer.dll") 
			{
				_mainPlayer.Stop();
				MarkPlaying(false);
			}
			//delPermanent = LOWORD(wParam)==IDM_DELETE_FOREVER;
			//::KillTimer(GetHWND(), delTimerID);
			//::SetTimer(GetHWND(), delTimerID, 150, (TIMERPROC)deleteFileProc);
			DeleteCurrentFile(LOWORD(wParam)==IDM_DELETE_FOREVER);
		} break;
		case IDM_DELETE_ALL_FOREVER: 
		if(!keyPressed) {
			//keyPressed = true;
			_mainPlayer.Stop();
			MarkPlaying(false);
			DeleteAllFile(true);
		} break;
		case IDM_COPY_IMAGE:
		if(!keyPressed) {
			//keyPressed = true;
			_mainPlayer.CopyImage();
		} break;
		case IDM_PASTE_PLAYLIST: 
		case IDM_APPEND_PLAYLIST: 
		if(!keyPressed) {
			//keyPressed = true;
			QkString data;
			bool purgeLst = LOWORD(wParam)==IDM_PASTE_PLAYLIST;
			GetTmpText(data, GetHWND());
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
					if(_playList[_playList.size()-1].StartWith(L"http")) { // name prev item
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
			//keyPressed = true;
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
			//keyPressed = true;
			QkString & text = _mainPlayer._currentPath;
			const TCHAR* pText = text;
			int length = text.GetLength();
			if(_osd) {
				InputBox* edit = dynamic_cast<InputBox*>(_osd->GetManager()->FindControl(_T("subtitle")));
				if(edit && edit->HasFocus()) {
					int ed=0;
					int st = edit->GetSel(&ed);
					if(st>=0 && st<ed) {
						//buff = edit->GetText().Mid(st, ed-st);
						pText = STR(edit->GetText())+st;
						length = ed-st;
					}
				}
			}
			SetTmpText(pText, length);
		} break;
		case IDM_COPY_PLAYLIST: 
		if(!keyPressed) {
			//keyPressed = true;
			int ct = _playList.size();
			std::wstring allPaths;
			int cc = selMap.size();
			if(selMap.size()) { // copy selected  && !IsKeyDown(VK_SHIFT)
				for (const auto& pair : selMap) 
				{
					allPaths += pair;
					if (--cc>0) // not the last item
					{
						allPaths += L"\r\n"; // use semicolon as separator
					}
				}
			} else { // copy all
				for (size_t i = 0; i < ct; i++)
				{
					allPaths += _playList[i];
					if (i != ct - 1) // not the last item
					{
						allPaths += L"\r\n"; // use semicolon as separator
					}
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

			SetTmpText(szText, allPaths.size());
			//if (OpenClipboard(NULL)) {
			//	EmptyClipboard();
			//	HANDLE hData = GlobalAlloc(GHND|GMEM_SHARE, (allPaths.size()+1)*sizeof(wchar_t));
			//	LPWSTR pData = (LPWSTR)GlobalLock(hData);
			//	CopyMemory(pData, szText, allPaths.size()*sizeof(wchar_t));
			//	pData[allPaths.size()] = L'\0';

			//	GlobalUnlock(hData);
			//	SetClipboardData(CF_UNICODETEXT, hData);
			//	CloseClipboard();
			//}
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
		case IDM_RESTORE:
			if(_isFullScreen) 
				ToggleFullScreen();
			if(_isMini) 
				ToggleMini();
			if(::IsMaximized(GetHWND()))
				SendMessage(WM_SYSCOMMAND, _isMaximized=SC_RESTORE, 0); 
			break;
		case IDM_MAX:
			if(keyPressed) break;
			//keyPressed = 1;
			if(_isFullScreen) 
				ToggleFullScreen();
			else if(::IsMaximized(GetHWND()))
				SendMessage(WM_SYSCOMMAND, _isMaximized=SC_RESTORE, 0); 
			else
				SendMessage(WM_SYSCOMMAND, _isMaximized=SC_MAXIMIZE, 0);
			m_pm._bStaticMovable = !(_isFullScreen||_isMaximized==SC_MAXIMIZE); 
			resetInset();
			break;
		case IDM_FULLSCREEN:
			if(keyPressed) break;
			//keyPressed = 1;
			ToggleFullScreen();
			ToggleFullScreen1();
			if (_pinBottom && !_isFullScreen) 
			{
				_bottomBar->SetVisible(1);
				::ShowWindow(_hFscBtmbar, SW_SHOW);
				makeTopmost(XPP->_bottomBar->GetHWND(), 1);
			}
			break;
		case IDM_SHUTDOWN:
			Close();
			break;
		case IDM_CLOSE:
			Close();
			break;

		case IDM_BKMK_ADD: {
			//WOD_IMG_UTILS("screenshotie", _mainPlayer._mMediaPlayer->getHWND());
			static int lastBkmkTm = 0;
			int now = GetTickCount();
			if (now - lastBkmkTm < 50) {
				lastBkmkTm = now;
				return -1;
			}
			lastBkmkTm = now;



			auto ret = _mainPlayer.AddBookmark();

			if (1) {
				auto bk = 0xFF0078d7;
				m_pm.GetRoot()->SetBkColor(0xffff0000);
				_mainPlayer.PostLambda([this,bk]()
					{
						m_pm.GetRoot()->SetBkColor(bk);
						return false;
					}, 350);
			}

			return ret;
		}
		case IDM_BKMK_DEL:
			return _mainPlayer.DelBookmark(_mainPlayer._selectedBookmark);
		case IDM_BKMK_COLOR_RED:
			return _mainPlayer.SetBookmarkColor(_mainPlayer._selectedBookmark, 0xffff0000);
		case IDM_BKMK_COLOR_GRE:
			return _mainPlayer.SetBookmarkColor(_mainPlayer._selectedBookmark, 0xff00ff00);
		case IDM_BKMK_COLOR_BLU:
			return _mainPlayer.SetBookmarkColor(_mainPlayer._selectedBookmark, 0xff0000ff);
		case IDM_BKMK_COLOR_WHI:
			return _mainPlayer.SetBookmarkColor(_mainPlayer._selectedBookmark, 0xffffffff);
		case IDM_BKMK_RETURN:
			_mainPlayer.SelectBookMark(_mainPlayer._selectedBookmark);
			return 1;
		case IDM_BKMK_RESTORE:
			_mainPlayer.SetPosition(_bakTime, true);
			return 1;
		case IDM_BKMK_RELOAD:
			_mainPlayer.LoadBookmarks();
			_mainPlayer._seekbar.Invalidate();
			return 1;
		case IDM_BKMK_VID: {
			//tmp.Format(L"%lld", LLONG_MAX-10);
			if (lParam==1) {
				QkString tmp;
				tmp.Format(L"%lld", _mainPlayer._timeMarked);
				SetTmpText(tmp);
			}
			return _mainPlayer._timeMarked;
		}
		case IDM_BKMK_EXPORT: {
			//tmp.Format(L"%lld", LLONG_MAX-10);
			auto& bkmks = _mainPlayer._bookmarks;
			QkString all_Str = L"";
			int cc = 0;
			for (size_t i = 0, size = bkmks.size(); i < size; i++)
			{
				auto& item = bkmks[i];
				if (lParam == 1 && item.layer!=0) {
					continue;
				}
				if(!all_Str.IsEmpty())
					all_Str += L"\n";
				all_Str.FormatEx(L"%d;%d;%d", true, item.pos, item.layer, item.color);
				cc++;
			}
			if (lParam == 1) {
				wofstream("R:\\cache\\wod_bm_export.txt") << STR(all_Str);
			}
			SetTmpText(all_Str);
			return cc;
		}

		case IDM_GET_PLAY:
			return _mainPlayer._isPlaying;
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

		case IDM_WINDOW_MODE:{
			UINT style = GetWindowLong(GetHWND(), GWL_STYLE);
			int ret = 0;
			if(style&WS_POPUP) ret |= 1<<3;
			if(::IsMaximized(GetHWND())) ret |= 1<<2;
			if(_isFullScreen) ret |= 1<<1;
			if(_isMini) ret |= 1;
			return ret;
		}
		case IDM_TEAM_UP:
			if(HIWORD(wParam)==0)
				_team.push_back((HWND)lParam);
			else {
				auto it = std::find(_team.begin(), _team.end(), (HWND)lParam);
				if (it != _team.end()) {
					_team.erase(it);
				}
			}
			break;
		case IDM_SINGLE_INSTANCE:
		{
			int sinst;
			if(lParam!=110) {
				sinst = (GetProfInt("sinst", 0)+1)%2;
				PutProfInt("sinst", sinst);
				closeMenus(true);
				//if(sinst && !hMutexAll) {
				//	hMutexAll = CreateMutex(NULL, TRUE, L"WODPL_SINST");
				//}
				//else if(!sinst && hMutexAll) {
				//	ReleaseMutex(hMutexAll);
				//	CloseHandle(hMutexAll);
				//	hMutexAll = 0;
				//}
			} else {
				sinst = (GetProfInt("sinst", 0))%2;
			}
			return sinst;
		}
		case IDM_SELECT_TOGGLE:
		{
			bool selected = selMap.find(XPP->_mainPlayer._currentPath)!=selMap.end();
			if(selected) selMap.erase(selMap.find(XPP->_mainPlayer._currentPath));
			else selMap.insert(XPP->_mainPlayer._currentPath);
			setSelIndi();
		} return selMap.size();
		case IDM_SELECT_ALL_TOGGLE:
		{
			bool nselected = selMap.find(XPP->_mainPlayer._currentPath)==selMap.end();
			if(nselected) {
				for (size_t i = 0; i < XPP->_playList.size(); i++)
				{
					if(selMap.find(XPP->_playList[i])!=selMap.end()) {
						nselected = false;
						break;
					}
				}
			}
			if(!nselected) selMap.clear();
			else {
				for (size_t i = 0; i < XPP->_playList.size(); i++)
				{
					selMap.insert(XPP->_playList[i]);
				}
			}
			setSelIndi();
		} return selMap.size();
		case IDM_PLAY_LOOP_ONE:
			if(lParam!=110) {
				_single = !_single;
				//lzz(%xud, _singleBtn->GetHotBkColor())
				_singleBtn->SetBkColor(_single?checkBG:0); // 0x34ffffff 0xff333333
				_singleBtn->SetHotBkColor(_single?checkBG:blueBG); // 0x34ffffff 0xff333333
			}
			return _single;
		case IDM_PLAY_FROM_MID:
			if(lParam!=110) {
				PutProfInt("mid", (GetProfInt("mid", 0)+1)%2);
			}
			closeMenus(true);
			return GetProfInt("mid", 0);
		case IDM_PLAY_TIME:
			return _mainPlayer.GetPosition(0);
		case IDM_PLAY_SEEK:
			_mainPlayer.SetPosition(lParam, HIWORD(wParam)==0);
			return 1;

		case IDM_SEEK_FORE: SeekDelta(1, 0); break;
		case IDM_SEEK_FORE_FAST: SeekDelta(1, 1); break;
		case IDM_SEEK_FORE_FASTER: SeekDelta(1, 2); break;
		case IDM_SEEK_BACK: SeekDelta(-1, 0); break;
		case IDM_SEEK_BACK_FAST: SeekDelta(-1, 1); break;
		case IDM_SEEK_BACK_FASTER: SeekDelta(-1, 2); break;

		case IDM_SEEK_BACK_FRAME: SeekEx(1); break;
		case IDM_SEEK_FORE_FRAME: SeekEx(2); break;
		case IDM_SEEK_BACK_KEYFRAME: SeekEx(3); break;
		case IDM_SEEK_FORE_KEYFRAME: SeekEx(4); break;

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

		case IDM_BKMK_PRV: return NavTimemark(-1);
		case IDM_BKMK_NXT: return NavTimemark(1);

		case IDM_PLAY_PRV:
		case IDM_PLAY_NXT:
			if(!keyPressed) {
				//keyPressed = true;
				NextPlayList(LOWORD(wParam)==IDM_PLAY_PRV, 0);
			}
			return 1;
		case IDM_PLAY_PRV_GP:
		case IDM_PLAY_NXT_GP:
			if(!keyPressed) {
				//keyPressed = true;
				NextPlayGroup(LOWORD(wParam)==IDM_PLAY_PRV_GP, 0);
			}
			return 1;
		case IDM_PLAY_A:
		case IDM_PLAY_Z:
			if(!keyPressed) {
				//keyPressed = true;
				int newIdx = (LOWORD(wParam)==IDM_PLAY_A?0:(_playList.size()-1));
				NavPlayList(newIdx);
				if (_mainPlayer._mMediaPlayer->IsPaused())
				{
					_mainPlayer._mMediaPlayer->Play();
				}
				MarkPlaying(true);
			}
			return 1;
		case IDM_PLAY_START: _lastSeekHome = GetTickCount64();  NavTime(-1); return 1;
		case IDM_PLAY_MID: NavTime(-2); return 1;
		case IDM_PLAY_END: _lastSeekEnd = GetTickCount64();  NavTime(-3); return 1;

		case IDM_PLAY_FASTBACKWARD:
			toggleFastforward(-5);
			break;
		case IDM_PLAY_FASTFOREWARD:
			toggleFastforward(5);
			break;
		case IDM_PLAY_FASTFASTBACKWARD:
			toggleFastforward(-15);
			break;
		case IDM_PLAY_FASTFASTFOREWARD:
			toggleFastforward(15);
			break;
		case IDM_PLAY_FASTFASTERBACKWARD:
			toggleFastforward(-30);
			break;
		case IDM_PLAY_FASTFASTERFOREWARD:
			toggleFastforward(30);
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
		case IDM_WIN:
			if(!HIWORD(wParam) || HIWORD(wParam)>=5)
			{
				trackWodMenus((CControlUI*)lParam, wParam);
				break;
			}
		case IDM_MINI:
			if (_isFullScreen) break;
			if(_isFullScreen) ToggleFullScreen();
			ToggleMini();
			break;
		case IDM_WIN_L:
			if(_isFullScreen) ToggleFullScreen();
			if(!_isMini) ToggleMini();
			{

			}
			break;
		case IDM_PIN:
		{	
			style = GetWindowLong(GetHWND(), GWL_EXSTYLE); // ex_style
			if(style&WS_POPUP) {
				SetWindowLong(GetHWND(), GWL_STYLE , style & ~WS_POPUP  | WS_THICKFRAME );
				style &= ~WS_EX_TOPMOST;
			}
			if(style&WS_EX_TOPMOST) {
				SetWindowLong(GetHWND(), GWL_EXSTYLE , style & ~WS_EX_TOPMOST );
				::SetWindowPos(GetHWND(), HWND_NOTOPMOST, 0, 0, 100, 100, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
			} else {
				SetWindowLong(GetHWND(), GWL_EXSTYLE , style | WS_EX_TOPMOST );
				::SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, 100, 100, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
			}
		}	break;
		case IDM_PIN_TOP:
		{
			UINT style = GetWindowLong(GetHWND(), GWL_STYLE);
			if(style&WS_POPUP) {
				SetWindowLong(GetHWND(), GWL_STYLE , style & ~WS_POPUP  | WS_THICKFRAME );

				SetWindowLong(GetHWND(), GWL_EXSTYLE , GetWindowLong(GetHWND(), GWL_EXSTYLE) & ~WS_EX_TOPMOST );
				::SetWindowPos(GetHWND(), HWND_NOTOPMOST, 0, 0, 100, 100, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
			} else {
				SetWindowLong(GetHWND(), GWL_STYLE , style & ~WS_THICKFRAME  | WS_POPUP );


				SetWindowLong(GetHWND(), GWL_EXSTYLE , GetWindowLong(GetHWND(), GWL_EXSTYLE) | WS_EX_TOPMOST );
				::SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, 100, 100, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);


				int w = ::GetSystemMetrics(SM_CXSCREEN)/2;
				int h = ::GetSystemMetrics(SM_CYSCREEN);
				//_lastTopMost = GetWindowLong(hWnd, GWL_EXSTYLE)&WS_EX_TOPMOST;
				RECT rc;
				GetWindowRect(GetHWND(), &rc);
				::SetWindowPos(hwnd, HWND_TOPMOST, rc.left, 0, XPP->m_pm.GetRoot()->GetWidth(), h, 0);
			}
			//SetWindowLong(GetHWND(), GWL_STYLE , style & ~dwNScStyle | WS_POPUP );
			SetFocus(XPP->GetHWND());
		} break;
		case IDM_FREEMOVE:
			_freeMove = !_freeMove;
			PutProfInt("free_move", _freeMove);
			closeMenus(1);
			break;
		case IDM_PINBOTTOM:
			_pinBottom = !_pinBottom;
			PutProfInt("pin_bottom", _pinBottom);
			closeMenus(1);
			break;
		case IDM_SKIN:
			if(HIWORD(wParam) && HIWORD(wParam)<5)
			{
				// 切换hollow
				PutProfInt("WndOp", GetProfInt("WndOp", -1)!=1?1:0);
				ResetWndOpacity();
				break;
			}
			trackWodMenus((CControlUI*)lParam, wParam);
			break;
		case IDM_PLUGIN:
			// 切换mpv+xunlei
			if(HIWORD(wParam) && HIWORD(wParam)<5)
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
				break;
			}
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
		case IDM_PLUGIN_MPV:
			PutProfString("player", "MPVExternalPlayer.dll");
			Replay();
			return 1;

		case IDM_SKIN_SEEKBAR_MAGNIFIER:
			if (_mag_type == 0) _mainPlayer._seekfloat.SetVisible(!_mainPlayer._seekfloat.IsVisible());
			else _mainPlayer._seekfloat.SetVisible(1);
			if (_mainPlayer._seekfloat.IsVisible()) _mag_type = 0;
			return 1;
		case IDM_SKIN_SEEKBAR_MAGNIFIER_BIG:
			if (_mag_type == 1) _mainPlayer._seekfloat.SetVisible(!_mainPlayer._seekfloat.IsVisible());
			else _mainPlayer._seekfloat.SetVisible(1);
			if (_mainPlayer._seekfloat.IsVisible()) _mag_type = 1;
			return 1;
			
		case IDM_MUTE_L:
			_muteL = !_muteL;
			if (_muteL && !IsKeyDown(VK_CONTROL)) _muteR = false;
			AdjustVoice();
			return 1;
		case IDM_MUTE_R:
			_muteR = !_muteR;
			if (_muteR && !IsKeyDown(VK_CONTROL)) _muteL = false;
			AdjustVoice();
			return 1;
		case IDM_MUTE_N:
			_muteR = 0;
			_muteL = 0;
			AdjustVoice();
			return 1;
		case IDM_MUSIC:
			_mainPlayer._mMediaPlayer->Command(20250910, 1); // music
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
		case NM_RELEASEDCAPTURE:
		{
			lzz(1)
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
	if (_safe_mode)
		return;
	BOOL bDelOK = 0;
	deleting = TRUE;
	if(permanent)
	{
		extern int MoveToVacuum(PCZZSTR path);
		QkString toDel = _mainPlayer._currentPath;
		//toDel.Prepend(L"\\\\?\\");
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
	deleting = FALSE;
}

void WODApplication::DeleteAllFile(BOOL permanent)
{
	if (_safe_mode) 
		return;
	auto & wod = XPP->_mainPlayer;
	auto & player = wod._mMediaPlayer;
	auto & lst = XPP->_playList;
	BOOL bDelOK = 0;
	if(permanent)
	{
		extern int MoveToVacuumW(LPCWSTR path);
		QkString toDel;
		string buffer;
		toDel.AsBuffer();
		vector<int> pos;
		for (size_t i = 0; i < lst.size(); i++)
		{
			if(PathFileExists(lst[i])) {
				toDel += lst[i];
				toDel += L'\0';
			}
		}
		toDel += L'\0';
		bDelOK = !MoveToVacuumW(toDel.GetData());
	}
	else
	{
		////std::this_thread::sleep_for(std::chrono::microseconds(1200));
		//extern int MoveToTrash(PCZZSTR path, BOOL bNoUI);
		//QkString toDel = _mainPlayer._currentPath;
		////toDel.Prepend(L"\\\\?\\");
		//toDel.Append(L"\0");
		//toDel.Append(L"\0");
		//std::string ppp  ;
		//toDel.GetData(ppp, 0);
		//int ret = MoveToTrash(ppp.data(), TRUE);
		////lxx(%ld , GetLastError() )
		////lxx(dd MoveToTrash, ret)
		//bDelOK = !ret;
	}
	if (bDelOK)
	{
		lst.clear();
	}
}



#include "WODLrc.hpp"

long lastFrom=-1;
long lastTo=-1;

void SyncSubtitles(long current) {
	//long current = XPP->_mainPlayer.GetPosition(false);

	if(current>=lastFrom && current<lastTo) {
		return;
	}
	if(_osd) {
		for (size_t i = 0; i < subtitles.size(); i++)
		{
			if(current>=subtitles[i].from && current<subtitles[i].to) {
				lastFrom = subtitles[i].from;
				lastTo = subtitles[i].to;
				//lxx(SyncSubtitles ss, STR(subtitles[i].line))

				InputBox* edit = dynamic_cast<InputBox*>(_osd->GetManager()->FindControl(_T("subtitle")));
				if(edit) {
					edit->SetText(subtitles[i].line);
					edit->SetScrollPos({0,0});
					edit->SetSel(0,0);
					//edit->NeedUpdate(); // todo optimize
					edit->NeedParentAutoUpdate();
					//edit->NeedParentUpdate();
				}
				break;
			}
		}
	}
}