#include "pch.h"
#include "WODMenus.h"

#include "PopupMenu.h"

extern WODApplication* XPP;

std::vector<PopupMenu*> MenuChain;

#pragma warning( disable : 26444 )

void initWodMenus(WODApplication * xpp)
{
	Menus = new std::vector<MenuDefine>{
		MenuDefine{L"file", L"文件", IDM_FILE, new std::vector<MenuDefine>{
			MenuDefine{L"open", L"打开(&O)", IDM_FILE_OPEN}
			, MenuDefine{L"close", L"关闭(F4)", IDM_FILE_CLOSE}
			, MenuDefine{L"recent", L"最近文件 asdasdasd", IDM_FILE_RECENT, new std::vector<MenuDefine>{
                    MenuDefine{L"open", L"最近文件1", IDM_FILE_OPEN}
                    , MenuDefine{L"open", L"最近文件2", IDM_FILE_CLOSE}
                    , MenuDefine{L"open", L"最近文件3", IDM_FILE_CLOSE, }
            }}
		}}
        , MenuDefine{L"bkmk", L"书签", IDM_BKMK, new std::vector<MenuDefine>{
            MenuDefine{L"bkmk_add", L"上一书签", IDM_BKMK_PRV}
            , MenuDefine{L"bkmk_add", L"下一书签", IDM_BKMK_NXT}
            , MenuDefine{L"", L"", 0}
            , MenuDefine{L"bkmk_add", L"添加书签", IDM_BKMK_ADD}
            , MenuDefine{L"", L"", 0}
            , MenuDefine{L"bkmk_add", L"删除书签", IDM_BKMK_DEL}
        }}
        , MenuDefine{L"plugin", L"插件", IDM_PLUGIN, new std::vector<MenuDefine>{
            MenuDefine{L"bkmk_add", L"Δ 原生MFPlayer", IDM_PLUGIN_MF}
            , MenuDefine{L"bkmk_add", L"Δ VLCPlayer", IDM_PLUGIN_VLC}
            , MenuDefine{L"bkmk_add", L"Δ 迅雷播放组件", IDM_PLUGIN_XL}
        }}
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
	};

    CHorizontalLayoutUI* menuBar = static_cast<CHorizontalLayoutUI*>(xpp->m_pm.FindControl(_T("menuBar")));
    if(menuBar) {
        menuBar->RemoveAll();
        for (size_t i = 0; i < Menus->size(); i++)
        {
            //auto menu = builder.Create(L"menu_item.xml", 0, 0, &m_pm);
            ////menu->SetFixedWidth(-1);
            //menu->GetText().Format(L"Menu#%d", i);
            //menuBar->Add(menu);
            auto menu = new Button();
            menu->SetText(Menus->at(i).name);
            menu->SetID(Menus->at(i).cmd);
            menu->SetTag(i);
            menu->SetRichEvent(true);
            menuBar->Add(menu);
            menu->SetAttribute(L"style", L"btn_wnd");
        }
    }
}


// 菜单适配器，负责显示列表、弹出子菜单。
class WODMenuAdapter : public MenuPopupAdapter 
{
    public:
        WODMenuAdapter(std::vector<MenuDefine>* scope, int index) 
            : scope(scope)
            , zindex(index)
        {
        }

        size_t GetItemCount() 
        {
            return scope->size();
        }

        CControlUI* CreateWndRoot(PopupMenu* popup, CPaintManagerUI* popupMan) override
        {
            CControlUI* ret = XPP->builder.Create(L"<VBox><ListView name=\"vList\"/></VBox>", TEXT("str"), 0, popupMan);
            popupMan->AttachDialog(ret);
            popup->pList = static_cast<ListView*>(popupMan->FindControl(_T("vList")));
            return ret;
        }

        bool EstimateSize(PopupMenu* menu, int & width, int & height) override
        {
            if (menu->pList)
            {
                auto refer = CreateItemView(menu->pList, 0);
                refer->SetFixedWidth(-2);
                menu->pList->SetAttribute(L"itemautoheight", L"true");
                menu->pList->SetReferenceItemView(refer);
                refer->SetManager(&menu->m_pm, 0, 0);
                int fat_row = 0, fat_row_wid=0, sep_row=0;
                for (size_t i = 0; i < scope->size(); i++)
                {
                    const QkString & name = scope->at(i).name;
                    if(name.GetLength() > fat_row_wid)
                    {
                        fat_row = i;
                        fat_row_wid = name.GetLength();
                    }
                    if(scope->at(i).id.IsEmpty())
                        sep_row++;
                }
                refer->SetText(scope->at(fat_row).name);
                refer->GetText().Append(L"XXX");

                SIZE sz = refer->EstimateSize({100, 100});
                sepHeight = 4;
                rowHeight = sz.cy;
                height = sz.cy*(GetItemCount() - sep_row) + sep_row * sepHeight;

                width = MAX(width, sz.cx);
                menu->pList->SetAdapter(this);
            }
            return true;
        }

        LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
        {
            int sz = MenuChain.size();
            int index = -1;
            for (size_t i = 0; i < sz; i++)
            {
                WODMenuAdapter* adapter = (WODMenuAdapter*)MenuChain[i]->_adapter.get();
                if(index>=0)
                {
                    MenuChain[i]->Close();
                }
                else if(MenuChain[i]->GetHWND()==::GetFocus()) 
                {
                    index = i;
                }
            }
            if(index >= 0)
            {
                MenuChain.resize(index+1);
                ::SetFocus(MenuChain[index]->GetHWND());
            }
            else
            {
                for (size_t i = 0; i < sz; i++)
                {
                    MenuChain[i]->Close();
                }
                MenuChain.resize(0);
            }
            return 0;
        }

        void Notify(TNotifyUI& msg) override
        {
            LogIs(L"Notify=%s", msg.sType);
            auto control = msg.pSender;
            if(msg.sType == _T("click")) 
            {
                LRESULT ret = ::SendMessage(XPP->m_pm.GetPaintWindow(), WM_COMMAND, control->GetID(), (LPARAM)msg.pSender);
                if(ret==1)
                    ::SetFocus(zindex>0?MenuChain[zindex-1]->GetHWND()
                        :XPP->GetHWND());
            }
            if(msg.sType == _T("mouseenter"))
            {
                int index = control->GetTag();
                int cmd = control->GetID();
                trackWodMenus(control, cmd, this->scope);
            }
        }

        CControlUI* CreateItemView(CControlUI* view, int type) override
        {
            CControlUI* ret = XPP->builder.Create(L"menu_row.xml", 0, 0, view->GetManager());
            ret->SetFixedWidth(-1);
            return ret;
        }

        void OnBindItemView(CControlUI* view, size_t index)
        {
            CControlUI* btn = dynamic_cast<CControlUI*>(view);
            if (btn)
            {
                btn->SetRichEvent(true);
                btn->SetID(scope->at(index).cmd);
                btn->SetTag(index);
                QkString & label = btn->GetText();
                label.AsBuffer();
                label = scope->at(index).name;
                btn->SetFixedHeight(scope->at(index).id.IsEmpty() ? sepHeight : rowHeight);
            }
        }
    public:
        const std::vector<MenuDefine>* scope; // display menu items
        const int zindex = 0; // which menu popup
        int rowHeight = 0;
        int sepHeight = 0;
};

bool trackWodMenus(CControlUI* control, int cmd, const std::vector<MenuDefine>* parentScope)
{
    bool tracked = false;
    if(control)
    {
        int sz = MenuChain.size();
        int index = -1;
        for (size_t i = 0; i < sz; i++)
        {
            WODMenuAdapter* adapter = (WODMenuAdapter*)MenuChain[i]->_adapter.get();
            if(index>=0)
            {
                MenuChain[i]->Close();
            }
            else if(adapter->scope==parentScope) 
            {
                index = i;
            }
        }
        bool switched = false;
        if(index >= 0)
        {
            switched = true;
            MenuChain.resize(index+1);
        }
        index = control->GetTag();
        if(index >= 0 && index < parentScope->size())
        {
            auto scope = parentScope->at(index).children;
            if(scope)
            {
                sz = MenuChain.size();
                MenuChain.push_back(new PopupMenu(cmd));
                PopupMenu* pMenu = MenuChain[sz];
                pMenu->_adapter = make_unique<WODMenuAdapter>(scope, sz);
                if(sz==0) 
                    pMenu->m_BasedPoint.y = control->GetHeight();
                else
                    pMenu->m_BasedPoint.x = control->GetWidth();
                pMenu->Init(control, &XPP->m_pm);
                tracked = true;
                //LogIs(2, "asdasd");
            }
        }
        if(!tracked && switched) {
            control->SetFocus();
        }
    }
    return tracked;
}