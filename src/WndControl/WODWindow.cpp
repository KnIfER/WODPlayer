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
#include "WODWindow.h"
#include "resource.h"
#include "SeekBar.h"
#include "VideoPlayer.h"
#include "InsituDebug.h"

HBRUSH bgBrush;

void WOD_Register(HINSTANCE hInstance)
{
	bgBrush = CreateSolidBrush (RGB(29,29,29));
	wchar_t lpszMenuName[] = L"Menu";

	static TCHAR szAppName[] = TEXT("Hello");
	MSG    msg;
	WNDCLASS wndclass;//WNDCLASSEX比WNDCLASS多两个结构成员－－cbSize(指定WNDCLASSEX结构的大小－－字节)  －－hIconSm(标识类的小图标)
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WODWindow::WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_MENU + 1);//白色 COLOR_WINDOW // COLOR_MENU 界面灰
	wndclass.hbrBackground = bgBrush;
	wndclass.lpszMenuName = lpszMenuName;
	wndclass.lpszMenuName = 0;
	wndclass.lpszClassName = szAppName;

	RegisterClass(&wndclass);
}

void WOD_UnRegister()
{
}

extern VideoPlayer* initVidePlayerImpl(WODWindow* xpp, int type);

void WODWindow::init(HINSTANCE hInstance, HWND hParent)
{
	WindowBase::init(hInstance, hParent);
	WOD_Register(hInstance);

	_hWnd = CreateWindowEx(WS_EX_ACCEPTFILES, L"Hello",      // window class name
		TEXT("The Hello Program"),   // window caption
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX  
		| WS_MAXIMIZEBOX | WS_SIZEBOX
		| WS_CLIPCHILDREN 
		,
		CW_USEDEFAULT,// initial x position
		CW_USEDEFAULT,// initial y position
		CW_USEDEFAULT,// initial x size
		CW_USEDEFAULT,// initial y size
		NULL,                 // parent window handle
		NULL,            // window menu handle
		hInstance,   // program instance handle
		this);      // creation parameters

	//_SysWndProc = (WNDPROC)GetWindowLongPtr(_hWnd, GWL_WNDPROC);

	//_SysWndProc = (WNDPROC)SetWindowLongPtr(_hWnd, GWL_WNDPROC, (LONG_PTR)WndProc);

	//_tabLayout = new TabLayout(hInstance, _hWnd);

	_toolbar.init(hInstance, _hWnd);

	//CreateRebar(_hWnd, _toolbar->getHWND());

	RECT rect;
	GetClientRect(_hWnd, &rect);

	MoveWindow(_toolbar.getHWND(), rect.left, 100+rect.top, rect.right, 24, true);

	SetWindowPos(_toolbar.getHWND(), 0 , 100+rect.left
		, 100+rect.top, rect.right, 12, SWP_SHOWWINDOW);


	_seekbar.init(hInstance, _hWnd);

	newVideoView();
}

void WODWindow::newVideoView()
{
	if (mMediaPlayer0)
	{
		delete mMediaPlayer0;
		mMediaPlayer0 = 0;
	}
	mMediaPlayer0 = initVidePlayerImpl(this, 0);
	if (mMediaPlayer0)
	{
		_hPlayer = mMediaPlayer0->getHWND();
	}
}

void WODWindow::showWindow()
{
	ShowWindow(_hWnd, SW_SHOW);
	UpdateWindow(_hWnd);
}

static DWORD dwNScStyle = WS_CAPTION|WS_BORDER|WS_SIZEBOX; 
RECT rcNScPos;


LRESULT WINAPI FullScreenBarsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void WODWindow::ToggleFullScreen()
{
	HWND hThisFSc = _hWnd;
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
			wc.hInstance = _hInst;
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
				_hWnd, NULL, NULL, this);
		}
		SetParent(_toolbar.getHWND(), _hFullScreenBtmbar);
		SetParent(_seekbar.getHWND(), _hFullScreenBtmbar);

		if (mMediaPlayer0) mMediaPlayer0->SetFullScreen(true);
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
		SetParent(_toolbar.getHWND(), _hWnd);
		SetParent(_seekbar.getHWND(), _hWnd);
		ShowWindow(_hFullScreenBtmbar, SW_HIDE);
		if (mMediaPlayer0) mMediaPlayer0->SetFullScreen(false);
		_isFullScreen = false;
		SetWindowLong(hThisFSc, GWL_STYLE , style|dwNScStyle );
		style = GetWindowLong(hThisFSc, GWL_EXSTYLE);
		SetWindowLong(hThisFSc, GWL_EXSTYLE, style&~WS_EX_TOPMOST);

		::SetWindowPos(hThisFSc, NULL, rcNScPos.left, rcNScPos.top
			, rcNScPos.right-rcNScPos.left, rcNScPos.bottom-rcNScPos.top, 0);
	}
}

bool WODWindow::IsFullScreen()
{
	return (GetWindowLong(_hWnd, GWL_STYLE)&WS_CAPTION)==0;
	//return _isFullScreen;
}

bool WODWindow::IsMediaPlayerWindow(HWND hwnd)
{
	return hwnd==_hPlayer||IsChild(_hPlayer, hwnd);
}

HWND WODWindow::GetMediaPlayerHWND()
{
	return _hPlayer;
}

void WODWindow::Toggle()
{
	if (mMediaPlayer0)
	{
		if (_isPlaying)
		{
			mMediaPlayer0->Pause();
		}
		else 
		{
			mMediaPlayer0->Play();
		}
		MarkPlaying(!_isPlaying);
	}
}

void WODWindow::MarkPlaying(bool playing)
{
	if (_isPlaying!=playing)
	{
		_isPlaying = playing;
		_toolbar.ReplaceIcon(0, _isPlaying?IDI_PAUSE:IDI_PLAY);
		RECT rc;
		GetClientRect(_toolbar.getHWND(), &rc);
		rc.right = rc.bottom;
		InvalidateRect(_toolbar.getHWND(), &rc, TRUE);
	}
}


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

bool WODWindow::PickFile()
{
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

	BOOL ret = PickFileDlg(_hWnd, FALSE, TEXT("选择媒体文件"), filters, filepath, MAX_PATH, NULL, 0);
	
	if (false)
	{
		newVideoView();
	}

	if(ret)
	{
		MarkPlaying(mMediaPlayer0->PlayVideoFile(filepath));
	}
	return false;
}

TCHAR nxt_file[_MAX_PATH];

extern bool running;

LRESULT WODWindow::RunProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static TCHAR s[] = TEXT("Hello, Windows.");
	switch (msg)
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
				if (mMediaPlayer0)
				{
					mMediaPlayer0->Close();
					lstrcpy(nxt_file, szFileName);
					::KillTimer(hwnd, 10086);
					::SetTimer(hwnd, 10086, 200, 0);
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

	case WM_CONTEXTMENU:
	{

		return 0;
	}

	case WM_CREATE:
	{

		MENUINFO mi = { 0 }; 
		mi.cbSize = sizeof(mi); 
		mi.fMask = MIM_BACKGROUND|MIM_APPLYTOSUBMENUS; 
		mi.hbrBack =  CreateSolidBrush (RGB(0,0,0)); 
		mi.dwStyle = MNS_AUTODISMISS;

		HMENU hMenu = ::GetMenu(hwnd); 
		SetMenuInfo(hMenu, &mi); 

		//test_menu_add_string(hMenu);
		//test_menualign(0);


		//test_menu_ownerdraw(hwnd, GetSubMenu(hMenu, 0));

		//test_menu_ownerdraw(hwnd, 0);

		return 0;
	}


	//case WM_PAINT:
	//{
	//
	//	HDC hdc;
	//	PAINTSTRUCT ps;
	//	RECT rect;
	//	GetClientRect(hwnd, &rect);
	//	hdc = BeginPaint(hwnd, &ps);
	//	DrawText (hdc, s, -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;
	//	EndPaint(hwnd, &ps);
	//	return 0;
	//}


	case WM_CLOSE:
	case WM_DESTROY:
		running = false;
		if (mMediaPlayer0)
		{
			//delete mMediaPlayer0;
			//mMediaPlayer0 = NULL;
		}
		_hWnd = 0;
		PostQuitMessage(0);
		return TRUE;
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		MoveWindow(_tabLayout.getHWND(), rect.left, rect.top, rect.right, rect.bottom, true);

		int videoHeightAvailable = rect.bottom - rect.top;
		int toolbarHeight = 0;
		int seekbarHeight = 0;
		bool fullScreen = IsFullScreen();
		//if (fullScreen)
		int barsHeight=0;

		if (1)
		{
			SendMessage(_toolbar.getHWND(), TB_AUTOSIZE, 0, 0);
			RECT toolbarRect;
			GetClientRect(_toolbar.getHWND(), &toolbarRect);
			toolbarHeight = toolbarRect.bottom-toolbarRect.top;
			barsHeight += toolbarHeight;
			//MoveWindow(_toolbar->getHWND(), 1000+rect.left, rect.top, rect.right, rect.bottom, true);

			//SetWindowPos(_toolbar.getHWND(), 0
			//	, 1000+rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
		}
		if (1)
		{
			seekbarHeight = 32;
			SetWindowPos(_seekbar.getHWND(), HWND_TOP, rect.left, rect.bottom-toolbarHeight-seekbarHeight, rect.right, seekbarHeight, SWP_SHOWWINDOW);

			barsHeight += seekbarHeight;
			//SetWindowPos(_toolbar->getHWND(), 0
			//	, 1000+rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
		}
		_barsHeight = barsHeight;
		if (mMediaPlayer0)
		{
			::SetWindowPos(mMediaPlayer0->getHWND(), HWND_TOP, 
				rect.left, 
				rect.top, 
				rect.right - rect.left, 
				fullScreen?videoHeightAvailable:(videoHeightAvailable-barsHeight), 
				SWP_SHOWWINDOW);
		}

		if (fullScreen)
		{
			SetWindowPos(_hFullScreenBtmbar , HWND_TOP
				, rect.left , rect.bottom-barsHeight, rect.right, barsHeight, SWP_SHOWWINDOW);
			SetWindowPos(_seekbar.getHWND(), HWND_TOP, rect.left, 0, rect.right, seekbarHeight, SWP_SHOWWINDOW);

			SendMessage(_toolbar.getHWND(), TB_AUTOSIZE, 0, 0);
		}
		return 0;
	}

	case WM_TIMER:
	{
		if (wParam==10086 && mMediaPlayer0)
		{
			mMediaPlayer0->PlayVideoFile(nxt_file);
			::KillTimer(hwnd, 10086);
		}
		if ((wParam == 1)
			&& (mMediaPlayer0->IsPlaying() || mMediaPlayer0->IsPaused()) )
		{
			long pos = mMediaPlayer0->GetPosition();
		
			//TCHAR szPosition[64];
			//TCHAR szDuration[64];
			//g_MyPlayer.MillisecondToText(g_MyPlayer.m_nPosition, szPosition);
			//g_MyPlayer.MillisecondToText(g_MyPlayer.m_nDuration, szDuration);
		
			//lstrcat(szPosition, _T("/"));
			//lstrcat(szPosition, szDuration);
		
			int nPos =  int (pos / (double)mMediaPlayer0->GetDuration() * 1000);
			//SendMessage(GetDlgItem(hwnd, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)nPos);
			
			if (!_seekbar._isSeeking)
			{
				_seekbar.SetPositionAndMax(pos, mMediaPlayer0->GetDuration());
			}

			//LogIs(3, "setPosition:: %d %d max=%d curr=%d\n", mMediaPlayer0->m_nPosition, mMediaPlayer0->m_nDuration, _seekbar.GetMax(), _seekbar.GetPosition());
		}
	}
		return 0;


	case WM_HSCROLL:
	{
		//LogIs("SetTime GETPOS:: %ld \n", _seekbar->GetPosition());
		if ((HWND)lParam==_seekbar.getHWND())
		{
			if (_seekbar._isSeeking && mMediaPlayer0
				&& (mMediaPlayer0->IsPlaying() || mMediaPlayer0->IsPaused()) )
			{
				mMediaPlayer0->SetPosition(wParam);
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
			Toggle();
		break;
		default:
			break;
		}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDM_START:
			Toggle();
			break;
		case IDM_STOP:
			if (mMediaPlayer0)
				mMediaPlayer0->Stop();
			MarkPlaying(false);
			break;
		case IDM_OPEN:
			PickFile();
			break;
		default:
			break;
		}

	}
	break;
	case MM_PREPARED:
	{
		LogIs("MPM_PREPARED %d\n", wParam);
		if (hwnd==_hWnd)
		{
			_seekbar.SetMax(wParam);
		}
		MarkPlaying(true);
		break;
	}
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

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}


LRESULT CALLBACK WODWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!running)
	{
		return TRUE;
	}
	if (!hwnd)
	{
		return FALSE;
	}
	else if(message==WM_NCCREATE)
	{
		WODWindow* app = (WODWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		app->_hWnd = hwnd;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)app);
	}
	return ((WODWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->RunProc(hwnd, message, wParam, lParam);
}