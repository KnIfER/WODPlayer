/** Copyright 2021 KnIfER JK. Chen
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
#pragma once
#include "WindowBase.h"

class ButtonList : public WindowBase 
{
public:
	ButtonList()=default;
	void init(HINSTANCE hInstance, HWND hParent);

	LRESULT RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
	
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ((ButtonList*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->RunProc(hwnd, message, wParam, lParam);
	}

	void showWindow();


	BOOL AddButtons(UINT buttonCount, LPTBBUTTON pButtonInfoArray) ;


	int GetCommandAt(int index);

	void AddIcon(int id);
	void ReplaceIcon(int pos, int id);



protected:
	HIMAGELIST _hImgList;

	int size;
};



