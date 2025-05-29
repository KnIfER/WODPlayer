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
        return CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW; 
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
    
    LRESULT HandleDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;


    void onPause(bool min=false);

    void onResume(bool min=false);

    LRESULT TimerProc();

    void ResetWndOpacity();

    void DeleteCurrentFile(BOOL permanent);

    void DeleteAllFile(BOOL permanent);

    void buildAccelerator();

    void resetInset();

    void ToggleFullScreen();
    void ToggleFullScreen1();
    void setMini();
    void ToggleMini();
    bool IsFullScreen();
    void MarkPlaying(bool playing=true);
    bool PickFile();

    void onNewVideo();

    WODPlayer _mainPlayer;
    WODPlayer _audioPlayer;
	//TabLayout _tabLayout;
	//ButtonList _toolbar;
    CControlUI* _playBtn;
	int _barsHeight=10;

    WinFrame* _bottomBar;
    CControlUI* _titleBar;
    CControlUI* _driveTag;
    CControlUI* _mimeTag;

    CControlUI* _topBarFscH;
    WinFrame* _topBarFscWnd;
    CControlUI* _topBar;

    WODBase* _db;
    Button* m_pSearch;

    WNDPROC _SysWndProc=NULL;
    bool _isFullScreen=false;
    bool _freeMove=false;
    bool _pinBottom=false;
    bool _isMini=false;
    int _isMaximized;

    int _WndOp;

    HACCEL _hAccTable;

    std::vector<QkString> _playList;
    int _playIdx;
    HWND _hFscBtmbar;

    vector<HWND> _team;
};
