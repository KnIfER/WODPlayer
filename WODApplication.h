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

class WODBase;

class WODApplication : public WindowImplBase, public INotifyUI
{
public:
    WODApplication();     
    ~WODApplication(){};     

    LPCTSTR GetWindowClassName() const override
    { 
        return _T("WODPlayer"); 
    }

    UINT GetClassStyle() const override
    { 
        return CS_DBLCLKS; 
    }

    LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) override;

    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) override;

    //LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

    void OnFinalMessage(HWND hWnd) override;

    //CControlUI* viewTemplate;

    void InitWindow() override;

    QkString GetSkinFile() override
    {
        return _T("WODPlayer.xml");
    }

    void Notify( TNotifyUI &msg ) override;

    CControlUI* CreateControl(LPCTSTR pstrClass) override;
    
    LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

    void ResetWndOpacity();

    void ToggleFullScreen();
    bool IsFullScreen();
    void MarkPlaying(bool playing=true);
    bool PickFile();

    WODPlayer _mainPlayer;
	//TabLayout _tabLayout;
	//ButtonList _toolbar;
    CControlUI* _playBtn;
	int _barsHeight=10;
	HWND _hFullScreenBtmbar;
    WODBase* _db;
    CDialogBuilder builder;
    Button* m_pSearch;

    WNDPROC _SysWndProc=NULL;
    bool _isFullScreen=false;
};
