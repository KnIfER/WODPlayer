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
#include "VideoPlayerInerface.h"



struct VWCreateOptions{
	HINSTANCE hPlugin=0;
	HINSTANCE hHost=0;
	HWND hParent=0;
	LONG_PTR* ret;
	const TCHAR* dllDir=0;
};

typedef int (__cdecl* VW_CREATEPLAYER)(VWCreateOptions);
typedef HWND (__cdecl* VW_GETHWND)(LONG_PTR);
typedef void (__cdecl* VW_RELEASE)(LONG_PTR);
typedef void (__cdecl* VW_PLAY)(LONG_PTR);
typedef void (__cdecl* VW_PAUSE)(LONG_PTR);
typedef void (__cdecl* VW_STOP)(LONG_PTR);
typedef bool (__cdecl* VW_ISPLAYING)(LONG_PTR);
typedef bool (__cdecl* VW_ISPAUSED)(LONG_PTR);
typedef long (__cdecl* VW_GETPOSITION)(LONG_PTR);
typedef long (__cdecl* VW_GETDURATION)(LONG_PTR);
typedef void (__cdecl* VW_SETPOSITION)(LONG_PTR, long);
typedef void (__cdecl* VW_SETFULLSCREEN)(LONG_PTR, bool);
typedef bool (__cdecl* VW_PLAYVIDEOFILE)(LONG_PTR, const TCHAR*);
typedef bool (__cdecl* VW_CLOSE)(LONG_PTR);
typedef void (__cdecl* VW_SYNCSIZE)(LONG_PTR, unsigned int*, unsigned int*);
typedef void (__cdecl* VW_INTERFACE)();
typedef float (__cdecl* VW_SETRATE)(LONG_PTR, float);
typedef int (__cdecl* VW_SETVOLUME)(LONG_PTR, int);

// D:\Code\FigureOut\XunLeiExternalPlayer\bin\XunLeiExternalPlayer.dll

class ExternalPlayer : public VideoPlayer
{
public:
	ExternalPlayer(int & error_code, HINSTANCE hInstance, HWND hParent, const TCHAR* dllPath, const TCHAR* dllDir=0);
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
	bool			PlayVideoFile(const TCHAR* path) override;
	void			Close() override;
	void			Release() override;
	void			syncResolution(unsigned int & _resX, unsigned int & _resY) override;
	float			SetRate(float rate) override;
	int			 SetVolume(int volume) override;
	HMODULE vwInit(int & error_code, const TCHAR* dllPath, bool blame=false, const TCHAR* dllDir=0);
protected:
	LONG_PTR _player;

	LRESULT RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ((ExternalPlayer*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->RunProc(hwnd, message, wParam, lParam);
	}
private:
	VW_CREATEPLAYER vwCreatePlayer = nullptr;
	VW_GETHWND vwGetHWND = nullptr;
	VW_RELEASE vwRelease = nullptr;
	VW_PLAY vwPlay = nullptr;
	VW_PAUSE vwPause = nullptr;
	VW_STOP vwStop = nullptr;
	VW_ISPLAYING vwIsPlaying = nullptr;
	VW_ISPAUSED vwIsPaused = nullptr;
	VW_GETPOSITION vwGetPosition = nullptr;
	VW_GETDURATION vwGetDuration = nullptr;
	VW_SETPOSITION vwSetPosition = nullptr;
	VW_SETFULLSCREEN vwSetFullScreen = nullptr;
	VW_PLAYVIDEOFILE vwPlayVideoFile = nullptr;
	VW_CLOSE vwClose = nullptr;
	VW_SYNCSIZE vwSyncSize = nullptr;
	VW_INTERFACE vwInterface = nullptr;
	VW_SETRATE vwSetRate = nullptr;
	VW_SETVOLUME vwSetVolume = nullptr;
};
