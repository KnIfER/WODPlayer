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
#pragma once
#include "WindowBase.h"
#include "TabLayout.h"
#include "ButtonList.h"
#include "VLCPlayer.h"
#include "SeekBar.h"

class WODWindow : public WindowBase 
{
public:
	WODWindow()=default;
	void init(HINSTANCE hInstance, HWND hParent);
	void newVideoView();

	LRESULT RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
	
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	void showWindow();

	void Toggle();
	HWND GetMediaPlayerHWND();
	bool IsMediaPlayerWindow(HWND hwnd);
	void ToggleFullScreen();
	bool IsFullScreen();
	void MarkPlaying(bool playing=true);
	bool PickFile();

	TabLayout _tabLayout;
	ButtonList _toolbar;
	SeekBar _seekbar;
	int _barsHeight=10;
	HWND _hFullScreenBtmbar;
protected:
	VideoPlayer* mMediaPlayer0=NULL;
	WNDPROC _SysWndProc=NULL;
	bool _isPlaying;
	bool _isFullScreen=false;
	HWND _hPlayer;
};



