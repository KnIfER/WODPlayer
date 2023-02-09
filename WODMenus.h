#pragma once

#include "WODMenuID.h"

struct MenuDefine{
	QkString id; // english
	QkString name; // localized
	UINT cmd; // command id
	std::vector<MenuDefine>* children = 0;
};


__declspec(selectany)  std::vector<MenuDefine>* Menus;

void initWodMenus(WODApplication* xpp);
