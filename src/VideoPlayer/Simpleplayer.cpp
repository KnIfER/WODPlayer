/** Copyright
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
#include "Simpleplayer.h"
#include "Resource.h"
#include "PlayerSink.h"
#include <CommCtrl.h>

#include <atlbase.h>
#include <atlhost.h>

#include "VideoPlayer.h"
#include "InsituDebug.h"

#define countof(x)			(sizeof(x)/sizeof(x[0]))

#define CONFIGID_PLAYRESULT		7		// Play result

#ifdef UNICODE
	#define lstrtoi		_wtoi
#else
	#define lstrtoi		atoi
#endif


CPlayerSink					g_APlayerSink;


HRESULT VPlayerXunBo::OnMessage(LONG nMessage, LONG wParam, LONG lParam)
{
	switch(nMessage)
	{
	case WM_SETFOCUS:
		LogIs(2, "WM_SETFOCUS");
		return TRUE;
	case WM_LBUTTONDBLCLK:
		break;

	case WM_KEYDOWN:
		switch(LOWORD(wParam))
		{
		case VK_ESCAPE:
			break;
		}
		break;
	}
	return S_OK;
}

int VPlayerXunBo::GetConfigInt(int nConfigId)
{
	USES_CONVERSION;
	_bstr_t strConfig = m_pAPlayer->GetConfig(nConfigId);
	LPTSTR pConfig = W2T(strConfig);
	int nResult = lstrtoi(pConfig);
	return nResult;
}

HRESULT VPlayerXunBo::OnStateChanged(LONG nOldState, LONG nNewState)
{
	TCHAR szText[MAX_PATH];
	if (nNewState==PS_CLOSING)
	{
		//m_pAPlayer->Play();
	}
	//HWND hWnd = GetDlgItem(g_hTextWnd, IDC_STATIC1);
	//
	//switch(nNewState)
	//{
	//case PS_READY:
	//	{
	//		DWORD dwPlayResult = g_MyPlayer.GetConfigInt(CONFIGID_PLAYRESULT);
	//		switch(dwPlayResult)
	//		{
	//		case S_OK:
	//			SetWindowText(hWnd, _T("播放完成！"));
	//			SetWindowText(GetDlgItem(g_hTextWnd, IDPAUSE), _T("播放"));
	//			break;
	//
	//		case S_FALSE:
	//			SetWindowText(hWnd, _T("准备就绪！"));
	//			break;
	//
	//		default:
	//			wsprintf(szText, _T("播放失败，错误码 0x%08X"), dwPlayResult);
	//			SetWindowText(hWnd, szText);
	//			break;
	//		}
	//	}
	//	break;
	//
	//case PS_OPENING:
	//	SetWindowText(hWnd, _T("正在打开..."));
	//	break;
	//
	//case PS_PAUSING:
	//	SetWindowText(hWnd, _T("正在暂停..."));
	//	break;
	//
	//case PS_PAUSED:
	//	break;
	//
	//case PS_PLAYING:
	//	SetWindowText(hWnd, _T("开始播放..."));
	//	break;
	//
	//case PS_PLAY:
	//	break;
	//
	//case PS_CLOSING:
	//	SetWindowText(hWnd, _T("正在关闭..."));
	//	break;
	//}

	return S_OK;
}

HRESULT VPlayerXunBo::OnOpenSucceeded()
{
	//SetWindowText(GetDlgItem(g_hTextWnd, IDPAUSE), _T("暂停"));
	//m_nDuration = m_pAPlayer->GetDuration();
	if (m_pAPlayer)
	{
		SendMessage(_hParent, MM_PREPARED, m_pAPlayer->GetDuration(), 0);
	}
	return S_OK;
}

HRESULT VPlayerXunBo::GetDuration()
{
	return m_pAPlayer?m_pAPlayer->GetDuration():0;
}

HRESULT VPlayerXunBo::OnSeekCompleted(LONG nPosition)
{
	return S_OK;
}

HRESULT VPlayerXunBo::OnBuffer(LONG nPercent)
{
	return S_OK;
}

HRESULT VPlayerXunBo::OnVideoSizeChanged(void)
{
	return S_OK;
}

HRESULT VPlayerXunBo::OnDownloadCodec(BSTR strCodecPath)
{
	// USES_CONVERSION;
	// TCHAR szText[MAX_PATH];
	// TCHAR szPath[MAX_PATH];
	// GetPathFromFullName(W2T(strCodecPath), szPath);
	// wsprintf(szText, _T("缺少解码器 %s, 下载后解压放在%s文件夹下。\n下载地址：http://aplayer.open.xunlei.com/codecs.zip"), W2T(strCodecPath), szPath);
	// MessageBox(g_hTextWnd, szText, _T("SimplePlayer"), MB_ICONINFORMATION);

	// 异步下载解码器模式，通知APlayer取消当前下载
	m_pAPlayer->SetConfig(19, _bstr_t("0"));
	return S_OK;
}

void VPlayerXunBo::Initialize()
{
	//m_nPosition = 0;
	//m_nDuration = 1000;
	m_pAPlayer = NULL;
	m_pConnectionPoint = NULL;
	m_dwCookie = 0;
	_hWnd = NULL;
	m_hModule = NULL;
}

void VPlayerXunBo::MillisecondToText(int nMs, TCHAR * pszText)
{
	int nSecond = nMs / 1000;
	int nMillisecond = nMs % 1000;

	int nHour = nSecond / 3600;
	nSecond = nSecond % 3600;
	int nMinute = nSecond / 60;
	nSecond = nSecond % 60;

	_stprintf(pszText, _T("%02d:%02d:%02d"), nHour, nMinute, nSecond);
}

BOOL VPlayerXunBo::GetPathFromFullName(const TCHAR * pcszFullName, TCHAR * pszPath)
{
	int nLength = lstrlen(pcszFullName);
	if(nLength >= MAX_PATH)
		return FALSE;
	lstrcpy(pszPath, pcszFullName);
	TCHAR * pEnd = pszPath + nLength - 1;
	while(pEnd > pszPath && *pEnd != '\\') pEnd--;
	*pEnd = '\0';
	return TRUE;
}

void VPlayerXunBo::Release()
{
	if (m_pAPlayer != NULL)
	{
		m_pAPlayer->Close();

		if (m_dwCookie != 0)
		{
			m_pConnectionPoint->Unadvise(m_dwCookie);
			m_dwCookie = 0;
		}

		m_pAPlayer->Release();
		m_pAPlayer = NULL;

		if (IsWindow(_hWnd))
		{
			DestroyWindow(_hWnd);
			_hWnd = NULL;
		}

		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}

HRESULT VPlayerXunBo::CreateInstanceFromFile(const TCHAR * pcszPath,
											REFCLSID rclsid,
											REFIID riid,
											IUnknown * pUnkOuter,
											LPVOID * ppv)
{
	m_hModule = ::LoadLibraryEx(pcszPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if(m_hModule == NULL)
		return TYPE_E_CANTLOADLIBRARY;

	typedef HRESULT (STDAPICALLTYPE *_pfnDllGetClassObject)(REFCLSID, REFIID, LPVOID*);
	_pfnDllGetClassObject pfnDllGetClassObject = (_pfnDllGetClassObject)GetProcAddress(m_hModule, "DllGetClassObject");
	if(pfnDllGetClassObject == NULL)
		return CO_E_ERRORINDLL;

	CComPtr<IClassFactory> spCF;
	HRESULT hr = (*pfnDllGetClassObject)(rclsid, IID_IClassFactory, (LPVOID*)&spCF);
	if(FAILED(hr))
		return hr;

	hr = spCF->CreateInstance(pUnkOuter, riid, ppv);
	if(FAILED(hr))
		return hr;

	spCF.Release();

	return S_OK;
}

BOOL VPlayerXunBo::CreateAPlayerWindow()
{
	TCHAR szFullName[MAX_PATH];
	GetModuleFileNameW(NULL, szFullName, MAX_PATH);
	TCHAR szPath[MAX_PATH];
	GetPathFromFullName(szFullName, szPath);
	wsprintf(szPath, _T("%s\\APlayer.dll"), szPath);
	HRESULT hr = CreateInstanceFromFile(szPath, 
										__uuidof(APlayer3Lib::Player), 
										__uuidof(APlayer3Lib::IPlayer), 
										NULL, 
										(void **)&m_pAPlayer);

	if (SUCCEEDED(hr))
	{
		// |WS_EX_NOACTIVATE
		_hWnd = ::CreateWindowEx(WS_EX_TOOLWINDOW, _T(ATLAXWIN_CLASS), _T(""),
										WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
										CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
										_hParent, NULL, NULL, NULL);
		// https://stackoverflow.com/questions/852856/win32-c-creating-a-popup-window-without-stealing-focus
		ShowWindow(_hWnd, SW_SHOWNOACTIVATE);
		SetWindowLong(_hWnd, GWLP_USERDATA, (LONG_PTR)this);
	}

	if (::IsWindow(_hWnd))
	{
		hr = AtlAxAttachControl(m_pAPlayer, _hWnd, NULL);
		_SysWndProc = (WNDPROC)SetWindowLong(_hWnd, GWLP_WNDPROC, (LONG_PTR)VPlayerXunBo::WndProc);
	}

	CComQIPtr<IConnectionPointContainer> spContainer;
	if (SUCCEEDED(hr))
	{
		hr = m_pAPlayer->QueryInterface(__uuidof(IConnectionPointContainer), 
										(void **)&spContainer);
	}

	if (SUCCEEDED(hr))
	{
		hr = spContainer->FindConnectionPoint(__uuidof(APlayer3Lib::_IPlayerEvents), 
												&m_pConnectionPoint);
	}

	if (SUCCEEDED(hr))
	{
		g_APlayerSink.SetEventHandler(this);
		hr = m_pConnectionPoint->Advise(&g_APlayerSink, &m_dwCookie);
	}

	// http://aplayer.open.xunlei.com/bbs/read.php?tid=468712
	m_pAPlayer->SetConfig(119, _bstr_t("1")) ;

	if (SUCCEEDED(hr))
		return TRUE;
	else
		return FALSE;
}


void VPlayerXunBo::Close()
{
	if (m_pAPlayer)
	{
		m_pAPlayer->Close();
	}
}
bool VPlayerXunBo::PlayVideoFile(TCHAR* path)
{
	if (m_pAPlayer == NULL)
	{
		TCHAR szFullName[MAX_PATH];
		GetModuleFileNameW(NULL, szFullName, MAX_PATH);
		TCHAR szPath[MAX_PATH];
		GetPathFromFullName(szFullName, szPath);
		wsprintf(szFullName, _T("请先下载APlayer SDK，并将其放解压放在%s文件夹下。下载地址：http://aplayer.open.xunlei.com/codecs.zip"), szPath);
		MessageBox(_hParent, szFullName, _T("SimplePlayer"), MB_ICONINFORMATION);
		return false;
	}

	m_pAPlayer->Open(path);
	return true;
}

void VPlayerXunBo::Play()
{
	if (m_pAPlayer != NULL)
	{
		m_pAPlayer->Play();
	}
}

void VPlayerXunBo::Pause()
{
	if (m_pAPlayer != NULL)
	{
		m_pAPlayer->Pause();
	}
}

bool VPlayerXunBo::IsPlaying()
{
	return m_pAPlayer!=NULL && m_pAPlayer->GetState()==PS_PLAY;
}

bool VPlayerXunBo::IsPaused()
{
	return m_pAPlayer!=NULL && m_pAPlayer->GetState()==PS_PAUSED;
}

void VPlayerXunBo::Stop()
{
	if (m_pAPlayer != NULL 
		&& (m_pAPlayer->GetState()==PS_PLAY||m_pAPlayer->GetState()==PS_PAUSED)
		)
	{
		m_pAPlayer->Close();
		{
			
			//SendMessage(GetDlgItem(hwnd, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
			//SetWindowText(GetDlgItem(hwnd, IDC_STATIC1), _T("00:00:00/00:00:00"));
			//SetWindowText(GetDlgItem(g_hTextWnd, IDPAUSE), _T("播放"));
		}
	}
}

long VPlayerXunBo::GetPosition()
{
	return m_pAPlayer?m_pAPlayer->GetPosition():0;
}

void VPlayerXunBo::SetPosition(long pos)
{
	if (m_pAPlayer)
	{
		m_pAPlayer->SetPosition(pos);
	}
}

// Sets the dialog box icons
inline void chSETDLGICONS(HWND hWnd, int idi)
{
	SendMessage(hWnd, WM_SETICON, ICON_BIG,  (LPARAM) 
		LoadIcon((HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE), 
		MAKEINTRESOURCE(idi)));
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM) 
		LoadIcon((HINSTANCE) GetWindowLongPtr(hWnd, GWLP_HINSTANCE), 
		MAKEINTRESOURCE(idi)));
}

BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{

	return(TRUE);
}

CComModule _module;

VPlayerXunBo::VPlayerXunBo(int & error_code, HINSTANCE hInstance, HWND hParent)
{
	error_code=1;
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	_module.Init(NULL, hInstance);

	AtlAxWinInit();

	WindowBase::init(hInstance, hParent);
	//g_hDialog = hwnd;

	//chSETDLGICONS(hwnd, IDI_ICON1);
	//SendMessage(GetDlgItem(hwnd, IDC_SLIDER1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 1000));

	SetTimer(hParent, 1, 1000, NULL);

	Initialize();
	if (CreateAPlayerWindow())
	{
		error_code=0;
	}
	else
	{
		//HWND hWnd = GetDlgItem(hwnd, IDC_STATIC1);
		//SetWindowText(hWnd, _T("加载APlayer失败！"));
		error_code=-1;
	}


	//DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_PLAYER), 
	//				NULL, Dlg_Proc, _ttoi(lpCmdLine));

}

VPlayerXunBo::~VPlayerXunBo()
{
	Release();
	m_pAPlayer = 0;
}

LRESULT VPlayerXunBo::RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	switch (msg)
	{
	case WM_SETFOCUS:
		LogIs(2, "WM_SETFOCUS");
		return TRUE;
	default:
		break;
	}
	return ::CallWindowProc(_SysWndProc, hwnd, msg, w, l);
}