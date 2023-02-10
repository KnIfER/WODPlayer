#include "pch.h"
#include "PopupMenu.h"

PopupMenu::PopupMenu(int cmd):
	//m_pOwner(NULL),
	//m_pLayout(),
	cmd(cmd)
	, isClosing(false)
	, m_dwAlignment(eMenuAlignment_Left | eMenuAlignment_Top)
{
	LogIs("PopupMenu::construct::%lxd", this);
	bAutoDel = true;
}

PopupMenu::~PopupMenu()
{
	LogIs("PopupMenu::destruct::%lxd", this);
	//Close();
}

void PopupMenu::Close(UINT nRet)
{
	ASSERT(::IsWindow(m_hWnd));
	isClosing = true;
	if (!::IsWindow(m_hWnd)) return;
	PostMessage(WM_CLOSE, (WPARAM)nRet, 0L);
}

// 静态创建方法，外部 -> Init
//    |pMenuCheckInfo| 默认 NULL
//    |dwAlignment| 默认 eMenuAlignment_Left | eMenuAlignment_Top
void PopupMenu::Init(CControlUI* pStickTo, CPaintManagerUI* manager)
{
	applicationMan = manager;
	_stickTo = pStickTo;
	//m_pLayout = NULL;
	//m_dwAlignment = dwAlignment;

	const RECT & rcStick = pStickTo->GetAbsolutePos();

	Create(/*m_pOwner?m_pOwner->GetManager()->GetPaintWindow()
		:*/manager->GetPaintWindow() , NULL, WS_POPUP , WS_EX_TOOLWINDOW | WS_EX_TOPMOST
		, CDuiRect(rcStick.left, rcStick.bottom, rcStick.left+100, rcStick.top+100));

	// HACK: Don't deselect the parent's caption
	HWND hWndParent = m_hWnd;
	while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
	::ShowWindow(m_hWnd, SW_SHOW);
	::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
}

LRESULT PopupMenu::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bool bShowShadow = false;
	m_pm.Init(m_hWnd);
	bShowShadow = m_pm.GetShadow()->IsShowShadow();
	CControlUI* pRoot = _adapter->CreateWndRoot(this, &m_pm);
	ASSERT(pRoot);
	m_pm.GetShadow()->ShowShadow(false);
	if(m_pm.GetRoot()!=pRoot) 
	{
		m_pm.AttachDialog(pRoot);
	}
	m_pm.AddNotifier(_adapter.get());
	int height = 50*10;
	int width = 100;

	RECT rc = _stickTo->GetAbsolutePos();
	if(!_adapter->EstimateSize(this, width, height)) 
	{
		if (pList)
		{
			auto refer = _adapter->CreateItemView(pList, 0);
			refer->SetFixedWidth(-1);
			//refer->SetFixedHeight(50);
			//pList->SetAttribute(L"itemautoheight", L"true");
			pList->SetReferenceItemView(refer);
			refer->SetManager(&m_pm, 0, 0);
			refer->GetText() = L"TEST";
			SIZE sz = refer->EstimateSize({100, 100});
			height = sz.cy*_adapter->GetItemCount();
			pList->SetAdapter(_adapter.get());
		}
		else 
		{
			SIZE sz = pRoot->EstimateSize({width, height});
			width = sz.cx;
			height = sz.cy;
		}
	}

	if(height > applicationMan->GetRoot()->GetHeight() - rc.top) 
	{
		RECT rect;  
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);  
		int cy = rect.bottom - rect.top;
		LogIs("height x=%d, height >=%d", height, cy - rc.top);
		if(height > cy - rc.top) {
			height = cy - rc.top;
			pList->SetAttribute(L"vscrollbar", L"true");
		}
	}

	rc.left += m_BasedPoint.x;//_stickTo->GetWidth();
	rc.top += m_BasedPoint.y;//_stickTo->GetHeight();


	SetForegroundWindow(m_hWnd);
	MoveWindow(m_hWnd, rc.left, rc.top, width, height, FALSE);
	return TRUE;
}

// 调整大小
void PopupMenu::ResizeMenu()
{
	CControlUI* pRoot = m_pm.GetRoot();

	pRoot->SetFixedXY({500,500});

	MONITORINFO oMonitor = {}; 
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;

	SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
	szAvailable = pRoot->EstimateSize(szAvailable);
	m_pm.SetInitSize(szAvailable.cx, szAvailable.cy);

	//必须是Menu标签作为xml的根节点
	CMenuUI *pMenuRoot = static_cast<CMenuUI*>(pRoot);
	ASSERT(pMenuRoot);

	SIZE szInit = m_pm.GetInitSize();
	szInit={100,100};
	CDuiRect rc;
	CDuiPoint point = m_BasedPoint;
	rc.left = point.x;
	rc.top = point.y;
	rc.right = rc.left + szInit.cx;
	rc.bottom = rc.top + szInit.cy;

	int nWidth = rc.GetWidth();
	int nHeight = rc.GetHeight();

	if (m_dwAlignment & eMenuAlignment_Right)
	{
		rc.right = point.x;
		rc.left = rc.right - nWidth;
	}

	if (m_dwAlignment & eMenuAlignment_Bottom)
	{
		rc.bottom = point.y;
		rc.top = rc.bottom - nHeight;
	}


	SetForegroundWindow(m_hWnd);
	MoveWindow(m_hWnd, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), FALSE);
	SetWindowPos(m_hWnd, HWND_TOPMOST, rc.left, rc.top, rc.GetWidth(), rc.GetHeight() + pMenuRoot->GetInset().bottom + pMenuRoot->GetInset().top, SWP_SHOWWINDOW);


	//pRoot->SetPos({0,0,rc.GetWidth(), rc.GetHeight()/2});
}

LPCTSTR PopupMenu::GetWindowClassName() const
{
	return _T("PlainPopupMenu");
}

//void PopupMenu::Notify(TNotifyUI& msg)
//{
//	_adapter->Notify();
//}

//CControlUI* PopupMenu::CreateControl( LPCTSTR pstrClassName )
//{
//	if (_tcsicmp(pstrClassName, _T("Menu")) == 0)
//	{
//		return new CMenuUI();
//	}
//	else if (_tcsicmp(pstrClassName, _T("MenuElement")) == 0)
//	{
//		return new CMenuElementUI();
//	}
//	return NULL;
//}

void PopupMenu::OnFinalMessage(HWND hWnd)
{
	//RemoveObserver();
	//if( m_pOwner ) {
	//	for( int i = 0; i < m_pOwner->GetSubMenuCount(); i++ ) {
	//		m_pOwner->GetSubMenuAt(i)->SetOwner(NULL);
	//	}
	//	m_pOwner->m_pWindow = NULL;
	//	m_pOwner->m_uButtonState &= ~ UISTATE_PUSHED;
	//	m_pOwner->Invalidate();

	//	// 内部创建的内部删除
	//	delete this;
	//}
}

void PopupMenu::setDPI(int DPI) {
	m_pm.SetDPI(DPI);
}

LRESULT PopupMenu::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//LogIs(2, "OnKillFocus");
	if(!isClosing)
		_adapter->OnKillFocus(uMsg, wParam, lParam, bHandled);
	return 0;
}

LRESULT PopupMenu::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//SIZE szRoundCorner = m_pm.GetRoundCorner();
	//if( !::IsIconic(*this) ) 
	//{
	//	CDuiRect rcWnd;
	//	::GetWindowRect(*this, &rcWnd);
	//	rcWnd.Offset(-rcWnd.left, -rcWnd.top);
	//	rcWnd.right++; rcWnd.bottom++;
	//	HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
	//	::SetWindowRgn(*this, hRgn, TRUE);
	//	::DeleteObject(hRgn);
	//}
	bHandled = FALSE;
	return 0;
}

LRESULT PopupMenu::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LogIs( "HandleMessage %d, %d", uMsg, uMsg==WM_CREATE);
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg )
	{
	case WM_CREATE: lRes = OnCreate(uMsg, wParam, lParam, bHandled);  break;
	case WM_KILLFOCUS: lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);  break;
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE || wParam == VK_LEFT)
			Close();
		break;
	case WM_SIZE: lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:
		//if( m_pOwner != NULL )
		//{
		//	m_pOwner->SetManager(m_pOwner->GetManager(), m_pOwner->GetParent(), false);
		//	m_pOwner->SetPos(m_pOwner->GetPos());
		//	m_pOwner->SetFocus();
		//}
		break;
	case WM_RBUTTONDOWN:
	case WM_CONTEXTMENU:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK: return 0L; break;
	default: bHandled = FALSE; break;
	}

	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return __super::HandleMessage(uMsg, wParam, lParam);
}


