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

class WODApplication;

class WODPlayer : public WinFrame 
{
public:
    WODPlayer() { };     

    void Toggle();
    HWND GetMediaPlayerHWND();
    bool IsMediaPlayerWindow(HWND hwnd);
    void MarkPlaying(bool playing=true);

    void newVideoView();

    void SetPos(RECT rc, bool bNeedInvalidate = true) override;

    SeekBar _seekbar;
    VideoPlayer* _mMediaPlayer;
//private:
    bool _isPlaying;
    HWND _hPlayer;
    WODApplication* _app;
};
