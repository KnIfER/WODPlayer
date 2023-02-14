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

#include "database\bookmark.h"

class WODApplication;

class WODPlayer : public WinFrame 
{
public:
    WODPlayer();     

    void Toggle();
    HWND GetMediaPlayerHWND();
    bool IsMediaPlayerWindow(HWND hwnd);
    void MarkPlaying(bool playing=true);


    bool PlayVideoFile(const TCHAR* path);
    bool AddBookmark();
    void SelectBookMark(int index);
    void DelBookmark(int index);

    void newVideoView();

    void Stop();
    void Release();

    float SpeedDelta(float delta);
    int SetVolume(int volume);


    void SetPos(RECT rc, bool bNeedInvalidate = true) override;
    bool HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& ret) override;


    SeekBar _seekbar;
    SeekBar _volumebar;
    VideoPlayer* _mMediaPlayer;
    QkString _currentPath;
//private:
    bool _isPlaying;
    HWND _hPlayer;
    RECT _exRect;
    WODApplication* _app;
    std::vector<BookMark> _bookmarks;
    int _selectedBookmark = -1;
    __int64 _timeMarked; // folder
    float speed = 1;
    int volume = 100;

    unsigned int _srcWidth;
    unsigned int _srcHeight;

    float _scale = 1;

    float _minScale = 1;
    bool _bNewImage = false;
    bool _bFit = true;

    int _translationX = 0;
    int _translationY = 0;
    bool _moving = false;
    int _moveLastX = 0;
    int _moveLastY = 0;
};
