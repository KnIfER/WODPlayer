#include "pch.h"
#include "WODMenus.h"

void initWodMenus(WODApplication * xpp)
{
	Menus = new std::vector<MenuDefine>{
		MenuDefine{L"file", L"文件", IDM_FILE, new std::vector<MenuDefine>{
			MenuDefine{L"open", L"打开(&O)", IDM_FILE_OPEN}
		}}
		, MenuDefine{L"bkmk", L"书签", IDM_BKMK}
		, MenuDefine{L"plugin", L"插件", IDM_PLUGIN}
	};
}