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
#include "windows.h"
#include "VLCPlayer.h"
#include "Resource.h"

#include "VideoPlayer.h"

typedef unsigned int ssize_t;
#include "vlc/vlc.h"

libvlc_instance_t *m_vlcInstance = NULL;
#define VPlayer (libvlc_media_player_t*)mMediaPlayer

extern HBRUSH bgBrush;
#define kClassWindow L"VideoFrame1"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
        LogIs("hookLButtonDown1");
        break;
    case WM_SIZE:
        break;
    case WM_DESTROY:
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

BOOL regWndClassWin(LPCTSTR lpcsClassName, DWORD dwStyle)
{
    WNDCLASS wndclass = { 0 };

    wndclass.style = dwStyle;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = ::GetModuleHandle(NULL);
    wndclass.hIcon = NULL;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = bgBrush;
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = lpcsClassName;

    ::RegisterClass(&wndclass);
    return TRUE;
}

BOOL unregWndClass(LPCTSTR lpcsClassName)
{
    ::UnregisterClass(lpcsClassName, ::GetModuleHandle(NULL));
    return TRUE;
}

int testVlc(HWND hwnd, CHAR* filePath)
{
    libvlc_media_player_t *m_vlcPlayer;

    m_vlcPlayer = NULL;

    if (m_vlcInstance==NULL)
    {
        const char** args = new const char*[] {
            //"-I", "dummy", "--ignore-config",
            "--no-video",
           // "--plugin-path=D:\plugins",
           // "--vout-filter=deinterlace", "--deinterlace-mode=blend"
           // "--vout-filter=deinterlace", "--deinterlace-mode=blend"
            "--vout-filter=deinterlace", "--no-audio"
        };

        m_vlcInstance = libvlc_new(3, args);
    }
    if (!m_vlcInstance)
    {
        LogIs(2, "init err!");
        return -1;
    }

    libvlc_media_t *vlcMedia = libvlc_media_new_path(m_vlcInstance , filePath);
    m_vlcPlayer = libvlc_media_player_new_from_media(vlcMedia);
    libvlc_media_release(vlcMedia);

    libvlc_media_player_set_hwnd(m_vlcPlayer, hwnd);
    libvlc_media_player_play(m_vlcPlayer);

    return 0;
}

libvlc_event_manager_t *eventManager;

// https://github.com/vlc-qt/vlc-qt/blob/master/src/core/MediaPlayer.cpp

static void handleEvents(const libvlc_event_t *event, void *userData)
{
    VLCPlayer *player = (VLCPlayer *)userData;
    switch (event->type) {
    case libvlc_MediaPlayerPositionChanged:
        break;
    case libvlc_MediaPlayerTimeChanged:
        LogIs("handleEvents::libvlc_MediaPlayerTimeChanged\n");
        PostMessage(player->getHParent(), WM_TIMER, 1, event->u.media_player_time_changed.new_time);
        break;
    case libvlc_MediaPlayerLengthChanged:
        LogIs("handleEvents::libvlc_MediaPlayerLengthChanged\n");
        PostMessage(player->getHParent(), MM_PREPARED
            , event->u.media_player_length_changed.new_length, 0);
        break;
    default:
        break;
    }
}


VLCPlayer::VLCPlayer(int & error_code, HINSTANCE hInstance, HWND hParent)
{
    error_code=1;
    WindowBase::init(hInstance, hParent);
    if (m_vlcInstance==NULL)
    {
        const char** args = new const char*[] {
            "-I", "dummy", "--ignore-config",
                // "--plugin-path=D:\plugins",
                "--vout-filter=deinterlace", "--deinterlace-mode=blend"
        };

        m_vlcInstance = libvlc_new(0, 0);
    }

    if (!m_vlcInstance)
    {
        return;
    }
    mMediaPlayer = (LONG_PTR)libvlc_media_player_new(m_vlcInstance);
    
    if (mMediaPlayer)
    {
        libvlc_video_set_key_input(VPlayer,false);
        libvlc_video_set_mouse_input(VPlayer,false);

        regWndClassWin(kClassWindow, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS);
        HWND hWnd = ::CreateWindowEx(WS_EX_TOOLWINDOW , kClassWindow , NULL
            , WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS
            , 0 , 0 , 840 , 680 , hParent , NULL , ::GetModuleHandle(NULL), NULL);
        if (hWnd)
        {

            _hWnd=hWnd;
            libvlc_media_player_set_hwnd(VPlayer, hWnd);
            eventManager = libvlc_media_player_event_manager(VPlayer);

            // 事件列表
            libvlc_event_e events[] {
                libvlc_MediaPlayerPositionChanged,
                libvlc_MediaPlayerTimeChanged,
                libvlc_MediaPlayerLengthChanged
            };

            // 订阅事件
            for (const libvlc_event_e &e : events) {
                libvlc_event_attach(eventManager, e, handleEvents, this);
            }

            error_code=0;
        }
    }
}

void VLCPlayer::Stop()
{
    libvlc_media_player_stop(VPlayer);
}

void VLCPlayer::Play()
{
    libvlc_media_player_play(VPlayer);

}

void VLCPlayer::Pause()
{
    libvlc_media_player_pause(VPlayer);
}

bool VLCPlayer::IsPlaying()
{
    return VPlayer&&libvlc_media_player_get_state(VPlayer)==libvlc_Playing;
}

bool VLCPlayer::IsPaused()
{
    return VPlayer&&libvlc_media_player_get_state(VPlayer)==libvlc_Paused;
}

long VLCPlayer::GetPosition()
{
    return libvlc_media_player_get_time(VPlayer);
}

void VLCPlayer::SetPosition(long pos)
{
    return libvlc_media_player_set_time(VPlayer, pos);
}

long VLCPlayer::GetDuration()
{
    return libvlc_media_player_get_length(VPlayer);
}

bool VLCPlayer::PlayVideoFile(TCHAR* path)
{
    if (!VPlayer)
    {
        return false;
    }
    CHAR buffer[256]={0};
    WideCharToMultiByte (CP_ACP, 0, path , -1, buffer, 256, 0, 0) ;

    libvlc_media_t *vlcMedia = libvlc_media_new_path(m_vlcInstance , buffer);
    libvlc_media_player_set_media(VPlayer, vlcMedia);
    Play();
    return true;
}

void VLCPlayer::SetFullScreen(bool val)
{
    libvlc_set_fullscreen(VPlayer, val);
}
