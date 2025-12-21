#include "pch.h"
#include "WODMenus.h"


extern WODApplication* XPP;


bool IsChecked(UINT id) {
    string* player;
    switch(id) {
        case IDM_FULLSCREEN: 
            return XPP->_isFullScreen;
        case IDM_SINGLE_INSTANCE: 
            return GetProfInt("sinst", 0);
        case IDM_PLAY_FROM_MID: 
            return GetProfInt("mid", 0);
        case IDM_MINI: 
            return !XPP->_isFullScreen && XPP->_isMini;
        //case IDM_PIN_TOP:
        case IDM_PIN:  {
            bool pin = GetWindowLong(XPP->GetHWND(), GWL_EXSTYLE)&WS_EX_TOPMOST;
            if(pin) {
                bool pin1 = GetWindowLong(XPP->GetHWND(), GWL_STYLE)&WS_POPUP;
                if(id==IDM_PIN) return !pin1;
                else return pin1;
            }
        }    return false;
        case IDM_PIN_TOP: {
            int h = ::GetSystemMetrics(SM_CYSCREEN);
            return XPP->m_pm.GetRoot()->GetHeight() >= h - 10;
        }    

        case IDM_FREEMOVE:
            return XPP->_freeMove;
        case IDM_PINBOTTOM:
            return XPP->_pinBottom;

        case IDM_PLUGIN_MF: 
            player = GetProfString("player");
            return player && *player=="MFExternalPlayer.dll";
        case IDM_PLUGIN_VLC: 
            player = GetProfString("player");
            return player && *player=="VLCExternalPlayer.dll";
        case IDM_PLUGIN_XL: 
            player = GetProfString("player");
            return player && *player=="XunLeiExternalPlayer\\XunLeiExternalPlayer.dll";
        case IDM_PLUGIN_MPV: 
            player = GetProfString("player");
            return player && *player=="MPVExternalPlayer.dll";
        case IDM_MUTE_L:
            return XPP->_muteL;
        case IDM_MUTE_R:
            return XPP->_muteR;
        case IDM_NO_DELELTE_TOGGLE:
            return XPP->_safe_mode;
        case IDM_BKMK_MUTE_TRACK:
            return XPP->_bkmk_add_layer==2;
        case IDM_BKMK_SEP_TRACK:
            return XPP->_bkmk_add_layer == 1;
        case IDM_BG_PLAY:
            return XPP->_bgPlay;
    }
    return false;
}


void initWodMenus(WODApplication * xpp)
{
    MainMenus = new std::vector<MenuDefine>{
		MenuDefine{L"file", L"文件", IDM_FILE, new std::vector<MenuDefine>{
                 MenuDefine{L"nodel", L"禁止删除", IDM_NO_DELELTE_TOGGLE, 0, true}
               , MenuDefine{L"open", L"打开(&O)", IDM_FILE_OPEN}
                 , MenuDefine{L"sinst", L"全局单实例(&S)", IDM_SINGLE_INSTANCE, 0, true}
			    , MenuDefine{L"mid", L"从中间开始(&M)", IDM_PLAY_FROM_MID, 0, true}
                , MenuDefine{L"audio", L"打开音轨(&A)", IDM_AUDIO_OPEN}
                , MenuDefine{L"audio", L"音轨(&M)", IDM_FILE_AUDIO, new std::vector<MenuDefine>{
                    MenuDefine{L"audio_open", L"打开…", IDM_AUDIO_OPEN}
                    , MenuDefine{L"", L"sep1", 0}
                }}
                , MenuDefine{L"bkmk", L"书签", IDM_BKMK, new std::vector<MenuDefine>{
                    MenuDefine{L"bkmk_add", L"上一书签", IDM_BKMK_PRV}
                    , MenuDefine{L"bkmk_add", L"下一书签", IDM_BKMK_NXT}
                    , MenuDefine{L"", L"", 0}
                    , MenuDefine{L"bkmk_add", L"添加书签", IDM_BKMK_ADD}
                    , MenuDefine{L"", L"", 0}
                    , MenuDefine{L"bkmk_add", L"删除书签", IDM_BKMK_DEL}
                }}
                , MenuDefine{L"", L"", 0}
			    , MenuDefine{L"recent", L"最近文件 asdasdasd", IDM_FILE_RECENT, new std::vector<MenuDefine>{
                        MenuDefine{L"open", L"最近文件1", IDM_FILE_OPEN}
                        , MenuDefine{L"open", L"最近文件2", IDM_FILE_CLOSE}
                        , MenuDefine{L"open", L"最近文件3", IDM_FILE_CLOSE, }
                }}
                //, MenuDefine{L"close", L"关闭(F4)", IDM_FILE_CLOSE}
		}}
        , MenuDefine{L"win", L"窗口", IDM_WIN, new std::vector<MenuDefine>{
           //MenuDefine{L"win1", L"迷你左分栏", IDM_WIN_L}
           // , MenuDefine{L"win2", L"迷你右分栏", IDM_WIN_R}
           // , MenuDefine{L"", L"", 0}
            MenuDefine{L"mini", L"免干扰", IDM_MINI, 0, true}
            , MenuDefine{L"", L"", 0}

           , MenuDefine{L"pin", L"置顶", IDM_PIN, 0, true}
            , MenuDefine{L"pin1", L"置顶（高覆盖任务栏）", IDM_PIN_TOP, 0, true}
            , MenuDefine{L"", L"", 0}
            , MenuDefine{L"fit", L"自由移动画面", IDM_FREEMOVE, 0, true}
            , MenuDefine{L"fit", L"始终显示底栏", IDM_PINBOTTOM, 0, true}
            , MenuDefine{L"win_nrm", L"复制图像", IDM_WIN_L}
            , MenuDefine{L"fit", L"适应视频大小", IDM_FIT}
            , MenuDefine{L"fit1", L"适应视频大小（自动）", IDM_FIT_AUTO, 0, true}
           //, MenuDefine{L"bkmk_add", L"右分屏", IDM_SPLIT}
           //, MenuDefine{L"", L"", 0}

            //, MenuDefine{L"", L"左分屏", IDM_SPLIT}
            //, MenuDefine{L"bkmk_add", L"右分屏", IDM_SPLIT}
            //, MenuDefine{L"", L"", 0}

            , MenuDefine{L"", L"", 0}

                , MenuDefine{L"skin", L"皮肤", IDM_SKIN, new std::vector<MenuDefine>{
                    MenuDefine{L"skin_norm", L"普通皮肤", IDM_SKIN_NORM}
                    , MenuDefine{L"skin_hollow", L"镂空", IDM_SKIN_HOLLOW}
                    , MenuDefine{L"skin_alpha_10s", L"透明度 100%", IDM_SKIN_ALPHA_10}
                    , MenuDefine{L"skin_alpha_9", L"透明度 90%",    IDM_SKIN_ALPHA_9}
                    , MenuDefine{L"skin_alpha_8", L"透明度 80%",    IDM_SKIN_ALPHA_8}
                    , MenuDefine{L"skin_alpha_7", L"透明度 70%",    IDM_SKIN_ALPHA_7}
                    , MenuDefine{L"skin_alpha_6", L"透明度 60%",    IDM_SKIN_ALPHA_6}
                    , MenuDefine{L"skin_alpha_5", L"透明度 50%",    IDM_SKIN_ALPHA_5}
                    , MenuDefine{L"skin_alpha_4", L"透明度 40%",    IDM_SKIN_ALPHA_4}
                    , MenuDefine{L"skin_alpha_3", L"透明度 30%",    IDM_SKIN_ALPHA_3}
                    , MenuDefine{L"skin_alpha_2", L"透明度 20%",    IDM_SKIN_ALPHA_2}
                    , MenuDefine{L"skin_alpha_1", L"透明度 10%",    IDM_SKIN_ALPHA_1}
                }}

                , MenuDefine{L"win_nrm", L"正常窗口", IDM_RESTORE}
                , MenuDefine{L"win_full", L"全屏", IDM_FULLSCREEN,0,1}
                , MenuDefine{L"win_max", L"最大化", IDM_MAXMISE}
        }}
        , MenuDefine{L"plugin", L"插件", IDM_PLUGIN, new std::vector<MenuDefine>{
            MenuDefine{L"bkmk_add", L"Δ MVP", IDM_PLUGIN_MPV, 0, 1}
            , MenuDefine{L"bkmk_add", L"Δ 迅雷播放组件", IDM_PLUGIN_XL, 0, 1}
            , MenuDefine{L"bkmk_add", L"Δ VLCPlayer", IDM_PLUGIN_VLC, 0, 1}
            , MenuDefine{L"bkmk_add", L"Δ 原生MFPlayer", IDM_PLUGIN_MF, 0, 1}
        }}
        , MenuDefine{L"voice", L"声音", IDM_PLUGIN, new std::vector<MenuDefine>{
            MenuDefine{L"mute_l", L"静音左声道", IDM_MUTE_L, 0, 1}
            , MenuDefine{L"mute_r", L"静音右声道", IDM_MUTE_R, 0, 1}
            , MenuDefine{L"mute_r", L"不静音", IDM_MUTE_N, 0, 0}
            , MenuDefine{L"music", L"仅播放声音", IDM_MUSIC, 0, 0}
            , MenuDefine{L"bg", L"后台播放", IDM_BG_PLAY, 0, 1}
            , MenuDefine{L"", L"", 0}
            , MenuDefine{L"mute_bkmk", L"静音书签", IDM_BKMK_MUTE_TRACK, 0, 1}
            , MenuDefine{L"mute_bkmk", L"章节书签", IDM_BKMK_SEP_TRACK, 0, 1}

        }}
	};

    // main menus
    CHorizontalLayoutUI* menuBar = static_cast<CHorizontalLayoutUI*>(xpp->m_pm.FindControl(_T("menuBar")));
    if(menuBar) {
        menuBar->RemoveAll();
        for (size_t i = 0; i < MainMenus->size(); i++)
        {
            //auto menu = builder.Create(L"menu_item.xml", 0, 0, &m_pm);
            ////menu->SetFixedWidth(-1);
            //menu->GetText().Format(L"Menu#%d", i);
            //menuBar->Add(menu);
            auto menu = new Button();
            menu->SetText(MainMenus->at(i).name);
            menu->SetID(MainMenus->at(i).cmd);
            menu->SetTag(i);
            menu->SetRichEvent(true);
            menuBar->Add(menu);
            menu->SetAttribute(L"style", L"btn_wnd");
            menu->SetFixedWidth(-2);
            menu->SetMinWidth(50);
            menu->isMenu = 1;
        }
    }


    MainMenuActor = (MenuActor)[](MenuDefine * item , int cmd, int pos)
    {
        if(item && !item->id.IsEmpty()) {
            if(cmd==0)
                return (int)IsChecked(item->cmd);
        }
        return 0;
    };
}


void trackWodMenus(CControlUI* btn, UINT cmd) 
{
    trackMenus(XPP, btn, (UINT)btn->GetTag());
}