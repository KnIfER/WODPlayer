/** Copyright 2023 WODPLayer author
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 3
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

#include <tchar.h>
#include <atlbase.h>
#include "WindowBase.h"
#include "VideoPlayerInerface.h"

class ExternalPlayer : public VideoPlayer
{
public:
	ExternalPlayer(int & error_code, HINSTANCE hInstance, HWND hParent, const TCHAR* dllPath);
	~ExternalPlayer();

public:
	void			Stop() override;
	void			Play() override;
	void			Pause() override;
	bool			IsPlaying() override;
	bool			IsPaused() override;
	long			GetPosition() override;
	long			GetDuration() override;
	void			SetPosition(long pos) override;
	bool			PlayVideoFile(TCHAR* path) override;
	void			Close() override;
	void			Release() override;
	void			syncResolution() override;
protected:
	LONG_PTR _player;

	LRESULT RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ((ExternalPlayer*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->RunProc(hwnd, message, wParam, lParam);
	}
};
