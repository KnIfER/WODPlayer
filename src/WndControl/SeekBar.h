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

class SeekBar : public WindowBase 
{
public:
	SeekBar()=default;
	void init(HINSTANCE hInstance, HWND hParent);

	LRESULT RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
	
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ((SeekBar*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->RunProc(hwnd, message, wParam, lParam);
	}

	void showWindow();

	void SetPosition(LONG pos);
	void SetPositionAndMax(LONG pos, LONG max);
	LONG GetPosition();
	void SetMax(LONG val);
	LONG GetMax();
	void SetMin(LONG val);
	LONG GetMin();

	bool _isSeeking=false;
protected:
	WNDPROC _SysWndProc=NULL;

	int size;
};



