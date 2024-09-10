#pragma once

#include "WODMenuID.h"
#include <iostream>

struct MenuDefine{
	QkString id; // english
	QkString name; // localized
	UINT cmd; // command id
	//unique_ptr<std::vector<MenuDefine>> children = 0;
	std::vector<MenuDefine>* children = 0;
	bool checkable = false;
	__int64 NPPs; // name path parts
};


__declspec(selectany)  std::vector<MenuDefine>* Menus;

void initWodMenus(WODApplication* xpp);

bool trackWodMenus(CControlUI* control, int cmd, int mainMenu=1, const std::vector<MenuDefine>* parentScope=Menus);

bool closeWodMenus(bool closeAll=false, HWND closeTill=nullptr);

