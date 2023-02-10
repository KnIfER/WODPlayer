#ifndef __PLAINPOPUPMENU_H__
#define __PLAINPOPUPMENU_H__

#pragma once

#include <iostream>

using namespace DuiLib;

	class PopupMenu;

class UILIB_API MenuPopupAdapter : public ListViewAdapter, public INotifyUI
{
	public:
		// 解析、创建布局
		virtual CControlUI* CreateWndRoot(PopupMenu* popup, CPaintManagerUI* popupMan)=0;
		virtual bool EstimateSize(PopupMenu* menu, int & width, int & height)=0;
		virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)=0;
};

class UILIB_API PopupMenu : public CWindowWnd 
{
	public:
		QkString GetSkinFile(){return L"";};
	public:
		PopupMenu(int cmd=0);
		~PopupMenu();

		void Close(UINT nRet = IDOK);
		bool isClosing;

		void Init(CControlUI* pStickTo, CPaintManagerUI* pMainPaintManager);
		void ResizeMenu();
		LPCTSTR GetWindowClassName() const;
		void OnFinalMessage(HWND hWnd);
		//void Notify(TNotifyUI& msg);
		//CControlUI* CreateControl(LPCTSTR pstrClassName);

		virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		void setDPI(int DPI);
	public:
		CControlUI* _stickTo;
		POINT			m_BasedPoint;
		//STRINGorID		m_xml;
		CPaintManagerUI m_pm;
		DWORD		m_dwAlignment;	//菜单对齐方式

		unique_ptr<MenuPopupAdapter> _adapter;

		ListView* pList;
		CPaintManagerUI* applicationMan;

		int cmd;
};

#endif // __UIMENU_H__