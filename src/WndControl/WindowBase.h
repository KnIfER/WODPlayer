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
#include <Windows.h>
#include <Windowsx.h>
#include "comctl32.h"

#define SendWndMessage(Msg, wParam, lParam) SendMessage(_hWnd, Msg, wParam, lParam)

class WindowBase 
{
public:
	HWND getHWND() {
		return _hWnd;
	};
	HWND getHParent() {
		return _hParent;
	};
	HINSTANCE getInstace() {
		return _hInst;
	};
	void init(HINSTANCE hInstance, HWND hParent) {
		_hInst=hInstance;
		_hParent=hParent;
	};
	LONG getHeight() {
		RECT rc;
		GetClientRect(_hWnd, &rc);
		return rc.bottom;
	};
	LONG getWidth() {
		RECT rc;
		GetClientRect(_hWnd, &rc);
		return rc.right;
	};
protected:
	HINSTANCE _hInst;
	HWND _hWnd;
	HWND _hParent;
	WNDPROC _SysWndProc=NULL;
};



