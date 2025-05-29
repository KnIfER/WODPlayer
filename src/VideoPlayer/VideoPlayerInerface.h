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

#define MM_PREPARED              (WM_USER)
#define MM_STOPPED               (WM_USER+1)

#include "../WndControl/WindowBase.h"

class VideoPlayer : public WindowBase
{
public:
	virtual void Play()=0;
	virtual void Stop()=0;
	virtual void Pause()=0;
	virtual bool IsPlaying()=0;
	virtual bool IsPaused()=0;
	virtual long GetPosition()=0;
	virtual void SetPosition(long pos, bool fastSeek)=0;
	virtual void SetLoop(bool loop)=0;
	virtual long GetDuration()=0;
	virtual bool PlayVideoFile(const TCHAR* path, const CHAR* path1)=0;
	virtual void SetFullScreen(bool val){};
	virtual void Release(){};
	virtual void Close(){};
	virtual void syncResolution(unsigned int & _resX, unsigned int & _resY){};
	virtual float SetRate(float rate)=0;
	virtual int SetVolume(int volume)=0;
	virtual void	SetRotation(int value) {}
	virtual int	GetRotation() { return 0; }
	virtual int	SetPositionEx(LONG wParam, LONG LPARAM) { return 0; }
	virtual int	CopyImage(const char* where) { return 0; }
	virtual LONG_PTR	Command(LONG wParam, LONG LPARAM,...) { return 0; }
	//POINT _resolution; 
	//unsigned int _resX; 
	//unsigned int _resY; 

};