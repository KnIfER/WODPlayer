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
#include <VideoPlayer.h>


class VLCPlayer : public VideoPlayer
{
public:
	VLCPlayer(int & error_code, HINSTANCE hInstance, HWND hParent);
	void			Stop() override;
	void			Play() override;
	void			Pause() override;
	bool			IsPlaying() override;
	bool			IsPaused() override;
	long			GetPosition() override;
	void			SetPosition(long pos) override;
	long			GetDuration() override;
	bool			PlayVideoFile(TCHAR* path) override;
	void			SetFullScreen(bool val) override;
private:
	LONG_PTR mMediaPlayer;
};