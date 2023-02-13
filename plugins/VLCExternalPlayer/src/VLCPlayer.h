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

//#include "WindowBase.h"
//#include <VideoPlayerInerface.h>


class VLCPlayer //: public VideoPlayer
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

public:
	VLCPlayer(int & error_code, HINSTANCE hPlugin, HINSTANCE hHost, HWND hParent);
	void			Stop() ;
	void			Release() ;
	void			Play() ;
	void			Pause() ;
	bool			IsPlaying() ;
	bool			IsPaused() ;
	long			GetPosition() ;
	void			SetPosition(long pos) ;
	long			GetDuration() ;
	bool			PlayVideoFile(const TCHAR* path) ;
	void			SetFullScreen(bool val) ;
	void			takeSnapShot(const char *psz_filepath);
	void			SyncSize(unsigned int * x, unsigned int * y) ;
	float  			SetRate(float rate);
	float		    SetVolume(int rate);
private:
	LONG_PTR mMediaPlayer;
};