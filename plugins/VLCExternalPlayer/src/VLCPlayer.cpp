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

#include "VLCPlayer.h"
//#include "Resource.h"

//#include "VideoPlayerInerface.h"
#include "InsituDebug.h"

//#include "DuiLib/UIlib.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "DuiLib\Utils\stb_image.h"

typedef unsigned int ssize_t;
#include "vlc/vlc.h"

#define MM_PREPARED              (WM_USER)
#define MM_STOPPED               (WM_USER+1)

libvlc_instance_t *m_vlcInstance = NULL;
#define VPlayer (libvlc_media_player_t*)mMediaPlayer

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
    LogIs("handleEvents::%d\n", event->type);
    VLCPlayer *player = (VLCPlayer *)userData;
    switch (event->type) {
    case libvlc_MediaPlayerVout:
        break;
    case libvlc_MediaPlayerPositionChanged:
        break;
    case libvlc_MediaPlayerTimeChanged:
        LogIs("handleEvents::libvlc_MediaPlayerTimeChanged\n");
        PostMessage(player->getHParent(), WM_TIMER, 1, event->u.media_player_time_changed.new_time);
        break;
    case libvlc_MediaPlayerStopped:
        PostMessage(player->getHParent(), MM_STOPPED
            , event->u.media_player_length_changed.new_length, 0);
        break;
    case libvlc_MediaPlayerPlaying:
    case libvlc_MediaPlayerLengthChanged:
        LogIs("handleEvents::libvlc_MediaPlayerLengthChanged\n");
        PostMessage(player->getHParent(), MM_PREPARED
            , event->u.media_player_length_changed.new_length, 0);
        break;
    case libvlc_MediaMetaChanged:
        LogIs("handleEvents::libvlc_MediaMetaChanged %s\n");
        //PostMessage(player->getHParent(), MM_PREPARED
        //    , event->u.media_player_length_changed.new_length, 0);
        break;
    default:
        break;
    }
}


VLCPlayer::VLCPlayer(int & error_code, HINSTANCE hPlugin, HINSTANCE hHost, HWND hParent)
{
    error_code=1;
    init(hPlugin, hParent);
    if (m_vlcInstance==NULL)
    {
        const char** args = new const char*[] {
            "-I", "dummy", "--ignore-config"
                 //"--plugin-path","C:\\Program Files (x86)\\VideoLAN\\vlc-3.0.18\\plugins"
                , "--vout-filter=deinterlace"
                , "--deinterlace-mode=blend"
                , "--input-repeat=2"
                //"vvv"
                //, "-vvv"
                //, "--vout","opengl"
                //, "--file-caching=0"
                //, "--clock-jitter=0"
                //, "--network-caching=0"
                //, "--sub-track=0"
                //, "--drop-late-frames"
                //, "--skip-frames"
                //, "--sout-rtp-proto=rtp"
                //, "--no-autoscale"


                    //, "--quiet"
                    //, "--no-sub-autodetect-file"
                    //, "--no-video"

                    //"--ignore-config", 
                    //"--no-osd",
                    //"--disable-screensaver",
                    //"--ffmpeg-hw",
                    //"--live-caching=100 ",
                    //"--rtsp-caching=100",
                    //"--realrtsp-caching=100",
                    //"--network-caching=0",

                    //"--skip-frames",
                    //"--drop-late-frames"

                , 0

        };
        //LogIs(2, "%d", sizeof(args));
        int cc=0;
        while(args[cc]) cc++;
        m_vlcInstance = libvlc_new(cc, args);
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
                libvlc_MediaPlayerStopped,
                libvlc_MediaPlayerPositionChanged,
                libvlc_MediaPlayerTimeChanged,
                libvlc_MediaMetaChanged ,
                libvlc_MediaPlayerLengthChanged
            };
            //libvlc_event_e events[] {
            //    libvlc_MediaMetaChanged,
            //    libvlc_MediaSubItemAdded,
            //    libvlc_MediaDurationChanged,
            //    libvlc_MediaParsedChanged,
            //    libvlc_MediaFreed,
            //    libvlc_MediaStateChanged,
            //    libvlc_MediaSubItemTreeAdded,

            //    libvlc_MediaPlayerMediaChanged,
            //    libvlc_MediaPlayerNothingSpecial,
            //    libvlc_MediaPlayerOpening,
            //    libvlc_MediaPlayerBuffering,
            //    libvlc_MediaPlayerPlaying,
            //    libvlc_MediaPlayerPaused,
            //    libvlc_MediaPlayerStopped,
            //    libvlc_MediaPlayerForward,
            //    libvlc_MediaPlayerBackward,
            //    libvlc_MediaPlayerEndReached,
            //    libvlc_MediaPlayerEncounteredError,
            //    libvlc_MediaPlayerTimeChanged,
            //    libvlc_MediaPlayerPositionChanged,
            //    libvlc_MediaPlayerSeekableChanged,
            //    libvlc_MediaPlayerPausableChanged,
            //    libvlc_MediaPlayerTitleChanged,
            //    libvlc_MediaPlayerSnapshotTaken,
            //    libvlc_MediaPlayerLengthChanged,
            //    libvlc_MediaPlayerVout,
            //    libvlc_MediaPlayerScrambledChanged,
            //    libvlc_MediaPlayerESAdded,
            //    libvlc_MediaPlayerESDeleted,
            //    libvlc_MediaPlayerESSelected,
            //    libvlc_MediaPlayerCorked,
            //    libvlc_MediaPlayerUncorked,
            //    libvlc_MediaPlayerMuted,
            //    libvlc_MediaPlayerUnmuted,
            //    libvlc_MediaPlayerAudioVolume,
            //    libvlc_MediaPlayerAudioDevice,
            //    libvlc_MediaPlayerChapterChanged,

            //    libvlc_MediaListItemAdded,
            //    libvlc_MediaListWillAddItem,
            //    libvlc_MediaListItemDeleted,
            //    libvlc_MediaListWillDeleteItem,
            //    libvlc_MediaListEndReached,

            //    libvlc_MediaListViewItemAdded,
            //    libvlc_MediaListViewWillAddItem,
            //    libvlc_MediaListViewItemDeleted,
            //    libvlc_MediaListViewWillDeleteItem,

            //    libvlc_MediaListPlayerPlayed,
            //    libvlc_MediaListPlayerNextItemSet,
            //    libvlc_MediaListPlayerStopped,

            //    /**
            //    * \deprecated Useless event, it will be triggered only when calling
            //    * libvlc_media_discoverer_start()
            //    */
            //    libvlc_MediaDiscovererStarted,
            //    /**
            //    * \deprecated Useless event, it will be triggered only when calling
            //    * libvlc_media_discoverer_stop()
            //    */
            //    libvlc_MediaDiscovererEnded,

            //    libvlc_RendererDiscovererItemAdded,
            //    libvlc_RendererDiscovererItemDeleted,

            //    libvlc_VlmMediaAdded,
            //    libvlc_VlmMediaRemoved,
            //    libvlc_VlmMediaChanged,
            //    libvlc_VlmMediaInstanceStarted,
            //    libvlc_VlmMediaInstanceStopped,
            //    libvlc_VlmMediaInstanceStatusInit,
            //    libvlc_VlmMediaInstanceStatusOpening,
            //    libvlc_VlmMediaInstanceStatusPlaying,
            //    libvlc_VlmMediaInstanceStatusPause,
            //    libvlc_VlmMediaInstanceStatusEnd,
            //    libvlc_VlmMediaInstanceStatusError
            //};

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


void VLCPlayer::Release()
{
    libvlc_media_player_stop(VPlayer);
    libvlc_media_player_release(VPlayer);
    libvlc_release(m_vlcInstance);
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

void VLCPlayer::SetPosition(long pos, bool fastSeek)
{
    return libvlc_media_player_set_time(VPlayer, pos);
}

long VLCPlayer::GetDuration()
{
    return libvlc_media_player_get_length(VPlayer);
}

bool VLCPlayer::PlayVideoFile(const CHAR* path)
{
    if (!VPlayer)
    {
        return false;
    }
    //CHAR buffer[256]={0};
    //WideCharToMultiByte (CP_ACP, 0, path , -1, buffer, 256, 0, 0) ;

    libvlc_media_t *vlcMedia = libvlc_media_new_path(m_vlcInstance , path);
    libvlc_media_player_set_media(VPlayer, vlcMedia);
    Play();
    return true;
}

void VLCPlayer::SetFullScreen(bool val)
{
    libvlc_set_fullscreen(VPlayer, val);
}

bool copyimage(const wchar_t* filename)
{
    //initialize Gdiplus once:
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    bool result = false;
    Gdiplus::Bitmap *gdibmp = Gdiplus::Bitmap::FromFile(filename);
    LogIs(2, "succ= %d  ", gdibmp);
    if (gdibmp)
    {
        HBITMAP hbitmap;
        gdibmp->GetHBITMAP(0, &hbitmap);
        if (OpenClipboard(NULL))
        {
            EmptyClipboard();
            DIBSECTION ds;
            if (GetObject(hbitmap, sizeof(DIBSECTION), &ds))
            {
                HDC hdc = GetDC(HWND_DESKTOP);
                //create compatible bitmap (get DDB from DIB)
                HBITMAP hbitmap_ddb = CreateDIBitmap(hdc, &ds.dsBmih, CBM_INIT,
                    ds.dsBm.bmBits, (BITMAPINFO*)&ds.dsBmih, DIB_RGB_COLORS);
                ReleaseDC(HWND_DESKTOP, hdc);
                SetClipboardData(CF_BITMAP, hbitmap_ddb);
                DeleteObject(hbitmap_ddb);
                result = true;
            }
            CloseClipboard();
        }

        //cleanup:
        DeleteObject(hbitmap);  
        delete gdibmp;              
    }
    return result;
}

bool copyimage(HBITMAP hbitmap)
{
    if (hbitmap && OpenClipboard(NULL)) {
        EmptyClipboard();
        SetClipboardData(CF_BITMAP, hbitmap);
        CloseClipboard();
        return 1;
    }
    return 0;
}

bool copyimage_1(HBITMAP hbitmap)
{
    if (hbitmap)
    {
        DIBSECTION ds;
        if (GetObject(hbitmap, sizeof(DIBSECTION), &ds))
        {
            HDC hdc = GetDC(HWND_DESKTOP);
            //create compatible bitmap (get DDB from DIB)
            HBITMAP hbitmap_ddb = CreateDIBitmap(hdc, &ds.dsBmih, CBM_INIT,
                ds.dsBm.bmBits, (BITMAPINFO*)&ds.dsBmih, DIB_RGB_COLORS);
            ReleaseDC(HWND_DESKTOP, hdc);
            copyimage(hbitmap_ddb);
            DeleteObject(hbitmap_ddb);
        }
        return 1;
    }
    return 0;
}

void VLCPlayer::takeSnapShot(const char* psz_filepath) {
    libvlc_video_take_snapshot(VPlayer, 0, psz_filepath, 0, 0);
}

void VLCPlayer::SyncSize(unsigned int * x, unsigned int * y) {
    libvlc_video_get_size(VPlayer, 0, x, y);
}

float VLCPlayer::SetRate(float rate)
{
    if(rate!=0 && libvlc_media_player_set_rate(VPlayer, rate)==0)
        return rate;
    return libvlc_media_player_get_rate(VPlayer);
}

float VLCPlayer::SetVolume(int value)
{
    if(value>=0 && libvlc_audio_set_volume(VPlayer, value)==0)
        return value;
    return libvlc_audio_get_volume(VPlayer);
}

void copyimage_1(const char* psz_filepath)
{
   //int x=0,y=0,n;
   //unsigned char *pImage = stbi_load(psz_filepath, &x, &y, &n, 0);

   //int bytesPerPixel = 3;
   //BITMAPINFO bmi;
   //::ZeroMemory(&bmi, sizeof(BITMAPINFO));
   //bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   //bmi.bmiHeader.biWidth = x;
   //bmi.bmiHeader.biHeight = y;
   //bmi.bmiHeader.biPlanes = 1;
   //bmi.bmiHeader.biCompression = BI_RGB;
   //bmi.bmiHeader.biBitCount = bytesPerPixel*8;
   //bmi.bmiHeader.biSizeImage = x * y * bytesPerPixel;

   //LPBYTE pDest = NULL;
   //HBITMAP hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
   ////hBitmap = CreateBitmap(100, 100, 1, 24, NULL);

   //BYTE alphaByte;
   //float alpha;
   //for( int i = 0; i < x * y; i++ ) 
   //{
   //    pDest[i*bytesPerPixel] = pImage[i*n + 2];
   //    pDest[i*bytesPerPixel + 1] = pImage[i*n + 1];
   //    pDest[i*bytesPerPixel + 2] = pImage[i*n]; 
   //    if (bytesPerPixel>3)
   //    {
   //        pDest[i*bytesPerPixel + 3] = alphaByte = pImage[i*n + 3];
   //        if (alphaByte<255)
   //        {
   //            alpha = alphaByte*1.f/255;
   //            pDest[i*bytesPerPixel] *= alpha;
   //            pDest[i*bytesPerPixel + 1] *= alpha;
   //            pDest[i*bytesPerPixel + 2] *= alpha; 
   //        }
   //    }
   //}
   // 
   //if(hBitmap) {
   //    stbi_image_free(pImage);

   //    bool succ = copyimage_1(hBitmap);

   //    DeleteObject(hBitmap);  

   //    LogIs(2, "succ = %d, n= %d", succ, n);
   //}

   //// copyimage(L"G:\\IMG\\123.png");
}

