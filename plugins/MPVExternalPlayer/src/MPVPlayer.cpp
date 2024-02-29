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
#include <Windows.h>
#include <GdiPlus.h>

#pragma comment(lib, "GdiPlus")//Visual Studio specific
#define STB_IMAGE_IMPLEMENTATION

#include "MPVPlayer.h"
//#include "Resource.h"

//#include "VideoPlayerInerface.h"
#include "InsituDebug.h"

//#include "DuiLib/UIlib.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "DuiLib\Utils\stb_image.h"

typedef unsigned int ssize_t;
#include <mpv/client.h>


#define MM_PREPARED              (WM_USER)
#define MM_STOPPED               (WM_USER+1)

mpv_handle *mpv = NULL;

#define kClassWindow L"VideoFrameMVP"

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
    wndclass.hbrBackground = CreateSolidBrush (RGB(29,29,29));
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
    //if (mpv==NULL)
    //{
    //    mpv = mpv_create();

    //}

    //if (!mpv)
    //{
    //    LogIs(2, "init err!");
    //    return -1;
    //}

    //// 设置 MPV 事件回调函数
    ////mpv_set_wakeup_callback(m_mpvInstance, handleMpvEvent, nullptr);

    //// 打开媒体文件
    //const char *file = "C:\\Users\\TEST\\Music\\20180620_175310.mp4";
    //const char** args = new const char*[] {
    //    "loadfile", file, NULL
    //};

    //mpv_set_option(mpv, "wid", mpv_format::MPV_FORMAT_INT64, &hwnd);
    //mpv_set_option_string(mpv, "loop", "inf");


    //mpv_initialize(mpv);

    //mpv_command(mpv, args);

    ////// 创建 MPV 渲染上下文
    ////mpv_render_context *mpvRenderContext;
    ////mpv_render_context_create(&mpvRenderContext, mpv, NULL);
    ////// 设置父窗口
    ////mpv_render_context_set_hwnd(mpvRenderContext, parentWindow);

    return 0;
}


//int main() 
//{
//    testVlc(0,0);
//}


// https://github.com/mpv-player/mpv-examples/tree/master/libmpv#methods-of-embedding-the-video-window


#include "windowsx.h"

static void handleEvents(mpv_event *event, MPVPlayer *player)
{
    LogIs("handleEvents::%d\n", event->event_id);
    switch (event->event_id) {
    case MPV_EVENT_PROPERTY_CHANGE:
        break;
    //case MPV_EVENT_TICK:
    //    LogIs("handleEvents::libvlc_MediaPlayerTimeChanged\n");
    //    PostMessage(player->getHParent(), WM_TIMER, 1, player->GetPosition());
    //    break;
    //case MPV_EVENT_IDLE:
    ////case MPV_EVENT_END_FILE:
    //    PostMessage(player->getHParent(), MM_STOPPED, 0, 0);
    //    break;
    //case MPV_EVENT_VIDEO_RECONFIG:
    //    LogIs("handleEvents::MPV_EVENT_VIDEO_RECONFIG %s\n");
    //    break;
    case MPV_EVENT_START_FILE:
        LogIs("handleEvents::MPV_EVENT_START_FILE %s\n");
        if(!player->getHWND()) {
            player->setHWND(::GetFirstChild(player->getHParent()));
        }
        PostMessage(player->getHParent(), MM_PREPARED, player->GetDuration(), 0);
        break;
    //case MPV_EVENT_VIDEO_RECONFIG:
    //    LogIs("handleEvents::libvlc_MediaMetaChanged %s\n");
    //    //PostMessage(player->getHParent(), MM_PREPARED
    //    //    , event->u.media_player_length_changed.new_length, 0);
    //    break;
    case MPV_EVENT_SHUTDOWN:
        mpv_terminate_destroy(mpv);
        mpv = NULL;
        break;
    default:
        break;
    }
}


static void wakeup(void *ctx){
    //MPVPlayer* player = (MPVPlayer*)ctx;
    //// 处理 MPV 事件
    //while (mpv) {
    //    LogIs("wakeup::\n");
    //    mpv_event *event = mpv_wait_event(mpv, 0);
    //    LogIs("wakeup::mpv_wait_event\n");
    //    if (!event || event->event_id == MPV_EVENT_NONE)
    //        break;
    //    handleEvents(event, player);
    //}
}

MPVPlayer::MPVPlayer(int & error_code, HINSTANCE hPlugin, HINSTANCE hHost, HWND hParent)
{
    error_code=1;
    init(hPlugin, hParent);
    //testVlc(hParent,0);
    if (mpv==NULL)
    {
        mpv = mpv_create();

    }

    if (!mpv)
    {
        LogIs(2, "init err!");
        return;
    }

    mpv_set_option(mpv, "wid", mpv_format::MPV_FORMAT_INT64, &hParent);
    mpv_set_option_string(mpv, "seekbarkeyframes", "yes");
    mpv_set_option_string(mpv, "loop", "inf");
    mpv_set_option_string(mpv, "keep-open", "yes");

    mpv_set_option_string(mpv, "force-window", "yes");
    mpv_set_option_string(mpv, "auto-window-resize", "no");
    //mpv_set_property_string(mpv, "af", "volume=10");
    mpv_set_property_string(mpv, "af", "lavfi=[pan=stereo|FL < 0.5*FC + 0.3*FLC + 0.3*FL + 0.3*BL + 0.3*SL + 0.5*LFE | FR < 0.5*FC + 0.3*FRC + 0.3*FR + 0.3*BR + 0.3*SR + 0.5*LFE],lavfi=[acompressor=10]");
    //mpv_set_property_string(mpv, "af", "dynaudnorm=f=200:g=7");
    //mpv_set_property_string(mpv, "af", "lavfi=[dynaudnorm=f=100:g=3]");

    mpv_set_property_string(mpv, "window-dragging", "no");
    mpv_set_property_string(mpv, "no-window-dragging", "yes");

    mpv_set_property_string(mpv, "af", "stats");

    //mpv_set_property_string(mpv, "hr-seek", "no");

    //mpv_set_option_string(mpv, "autofit", "no");
    //mpv_set_option_string(mpv, "keepaspect", "no");
    //mpv_set_option_string(mpv, "keepaspect-window", "no");

    //mpv_set_option(m_mpvInstance, "ctx", mpv_format::MPV_FORMAT_INT64, this);

    // 设置 MPV 事件回调函数
    mpv_set_wakeup_callback(mpv, wakeup, this);

    mpv_initialize(mpv);


    //tg
    //SetRotation(125);


    error_code = 0;
}

void MPVPlayer::Stop()
{
    if(mpv) mpv_command_string(mpv, "stop");
}


void MPVPlayer::Release()
{
    if(mpv) mpv_terminate_destroy(mpv);
    mpv = 0;
}

void MPVPlayer::Play()
{
    const char *pause_command = "no";
    mpv_set_property_string(mpv, "pause", pause_command);
}

void MPVPlayer::Pause()
{
    const char *pause_command = "yes";
    mpv_set_property_string(mpv, "pause", pause_command);
}

bool MPVPlayer::IsPlaying()
{
    int playback_state;
    if (mpv_get_property(mpv, "pause", MPV_FORMAT_FLAG, &playback_state) < 0)
        return false;
    return !playback_state && GetDuration()>0;
}

bool MPVPlayer::IsPaused()
{
    int playback_state;
    mpv_get_property(mpv, "pause", MPV_FORMAT_FLAG, &playback_state);
    return playback_state && GetDuration()>0;
}

long MPVPlayer::GetPosition()
{
    double duration;
    int error = mpv_get_property(mpv, "time-pos", MPV_FORMAT_DOUBLE, &duration);
    return duration*1000;
}

#include <stdio.h>

CHAR buffer[256]={0};

void MPVPlayer::SetPosition(long pos, bool fastSeek)
{
    if(fastSeek) 
    {
        sprintf(buffer, "%.3f", pos/1000.0);
        const char** args = new const char*[] {
            "seek", buffer
                // , "exact", "synchronous"
                , "absolute"
                , NULL
        };
        mpv_command(mpv, args);
    }
    else
    {
        double v = pos/1000.0;
        mpv_set_property(mpv, "time-pos", MPV_FORMAT_DOUBLE, &v);
    }

//    sprintf(buffer, "%.3f", pos/1000.0);
//    const char** args = new const char*[] {
//        "seek", buffer
//            // , "exact", "synchronous"
//            , "absolute+exact"
//            , NULL
//    };
//    mpv_command(mpv, args);

    //sprintf(buffer, "%.3f", pos/1000.0);
    //mpv_set_property_string(mpv, "time-pos", (const char *)buffer);
}

void MPVPlayer::SetLoop(bool val)
{
    mpv_set_option_string(mpv, "loop", val?"inf":"no");
}

void MPVPlayer::SetInitialPosition(long pos)
{
    //mpv_set_option_string(mpv, "start", "00:04");
}

long MPVPlayer::GetDuration()
{
    double duration;
    int error = mpv_get_property(mpv, "duration", MPV_FORMAT_DOUBLE, &duration);
    return duration*1000;
}

bool MPVPlayer::PlayVideoFile(const CHAR* path)
{
    const char** args = new const char*[] {
        "loadfile"
            , path
            , NULL
    };
    mpv_command(mpv, args);

    //mpv_set_property_string(mpv, "stream-record", "G:\\BaiduNetdiskDownload\\New_folder\\test.flv");
    //, "--"
    //, 
    return true;
}

void MPVPlayer::SetFullScreen(bool val)
{

}

void MPVPlayer::takeSnapShot(const char* psz_filepath) {

}

void MPVPlayer::SyncSize(unsigned int * x, unsigned int * y) {
    MPVPlayer* player = this;//(MPVPlayer*)ctx;
    int64_t w=*x, h=*y;
    mpv_get_property(mpv, "dwidth", MPV_FORMAT_INT64, &w);
    mpv_get_property(mpv, "dheight", MPV_FORMAT_INT64, &h);

    if (mRotation && ((mRotation%180>=45 || mRotation%180<=135)))
    {
        *x=h; *y=w;
    }
    else
    {
        *x=w; *y=h;
    }

    //LogIs("syncResolution x=%d y=%d\n", *x, *y);
    // 处理 MPV 事件
    while (mpv) {
        //LogIs("wakeup::\n");
        mpv_event *event = mpv_wait_event(mpv, 0);
        //LogIs("wakeup::mpv_wait_event\n");
        if (!event || event->event_id == MPV_EVENT_NONE)
            break;
        handleEvents(event, player);
    }
}

float MPVPlayer::SetRate(float value)
{
    //_itoa(value, buffer, 10);
    //if(mpv) mpv_set_property_string(mpv, "rate", buffer);
    double v = value;
    mpv_set_property(mpv, "speed", MPV_FORMAT_DOUBLE, &v);
    mpv_get_property(mpv, "speed", MPV_FORMAT_DOUBLE, &v);
    return v;
}

float MPVPlayer::SetVolume(int value)
{
    _itoa(value, buffer, 10);
    if(mpv) mpv_set_property_string(mpv, "volume", buffer);
    return 1;
}

void MPVPlayer::SetRotation(int value)
{
    if (mRotation!=value)
    {
        //mpv_set_property_string(mpv, "video-rotate", "45");
        mRotation = value;
        mpv_set_property(mpv, "video-rotate", MPV_FORMAT_INT64, &mRotation);
    }
}

int MPVPlayer::GetRotation()
{
    //mpv_get_property_string(mpv, "video-rotate", "45");
    mpv_get_property(mpv, "video-rotate", MPV_FORMAT_INT64, &mRotation);
    return mRotation;
}
