//------------------------------------------------------------------------------
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//------------------------------------------------------------------------------
// https://github.com/microsoft/Windows-classic-samples/tree/master/Samples/Win7Samples/multimedia/mediafoundation/MFPlayer2

#pragma once

#include "AudioSessionVolume.h"

//#include "VideoPlayer.h"

#include <mfplay.h>


// Private window message to notify the application of playback events.
static const UINT WM_APP_NOTIFY = WM_APP + 1;   // wparam = MFP_MEDIAPLAYER_STATE

// Private window message to notify the application when an error occurs.
static const UINT WM_APP_ERROR = WM_APP + 2;    // wparam = HRESULT

// Private window message to notify the application of an audio session event.
static const UINT WM_AUDIO_EVENT = WM_APP + 4;

class MFPlayer2 : public IMFPMediaPlayerCallback//, public VideoPlayer
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
    // Constructor is private. Call CreateSamplePlayer to instantiate.
    MFPlayer2(int & error_code, HINSTANCE hInstance, HWND hParent);

    // Destructor is private. Application must call Release().
    ~MFPlayer2();

    // IUnknown methods
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IMFPMediaPlayerCallback methods
    void STDMETHODCALLTYPE OnMediaPlayerEvent(MFP_EVENT_HEADER * pEventHeader);

    // Player methods

    // Playback
    HRESULT OpenURL(const WCHAR *sURL);

    void			Stop() ;
    void			Play() ;
    void			Pause() ;
    bool			IsPlaying() ;
    bool			IsPaused() ;
    long			GetPosition() ;
    void			SetPosition(long pos) ;
    long			GetDuration() ;
    bool			PlayVideoFile(const TCHAR* path) ;
    void			SetFullScreen(bool val){} ;
    void			takeSnapShot(const char *psz_filepath){};
    void			SynSize(unsigned int * x, unsigned int * y);

    HRESULT Shutdown();
    HRESULT GetState(MFP_MEDIAPLAYER_STATE *pState);
    HRESULT GetMetadata(IPropertyStore **ppProp);

    // Video
    HRESULT HasVideo(BOOL *pfHasVideo);
    HRESULT SetZoom(float fZoom);
    HRESULT UpdateVideo();

    // Audio
    BOOL    IsAudioEnabled() const { return (m_pVolume != NULL); }
    HRESULT SetVolume(float fLevel);
    HRESULT GetVolume(float *pfLevel);
    HRESULT SetMute(BOOL bMute);
    HRESULT GetMute(BOOL *pbMute);

    // Seeking
    HRESULT GetDuration(MFTIME *phnsDuration);
    HRESULT CanSeek(BOOL *pbCanSeek);
    HRESULT GetCurrentPosition(MFTIME *phnsPosition);
    HRESULT SetPosition(MFTIME hnsPosition);

    // Fast forward / rewind / Frame step
    HRESULT CanFastForward(BOOL *pbCanFF);
    HRESULT CanRewind(BOOL *pbCanRewind);
    HRESULT SetPlaybackRate(float fRate);
    HRESULT FastForward();
    HRESULT Rewind();
    HRESULT FrameStep();

    // Effects
    HRESULT SetEffect(IMFTransform *pMFT);

protected:
    // NotifyState: Notifies the application when the state changes.
    void NotifyState(MFP_MEDIAPLAYER_STATE state)
    {
        PostMessage(m_hwndEvent, WM_APP_NOTIFY, (WPARAM)state, (LPARAM)0);
    }

    // NotifyError: Notifies the application when an error occurs.
    void NotifyError(HRESULT hr)
    {
        PostMessage(m_hwndEvent, WM_APP_ERROR, (WPARAM)hr, 0);
    }

    HRESULT SetZoom();

    float   GetNominalRate() { return m_fRate; }

    // MFPlay event handler functions.
    void OnMediaItemCreated(MFP_MEDIAITEM_CREATED_EVENT *pEvent);
    void OnMediaItemSet(MFP_MEDIAITEM_SET_EVENT *pEvent);
    void OnRateSet(MFP_RATE_SET_EVENT *pEvent);

protected:
    long                m_cRef;        // Reference count.
    IMFPMediaPlayer     *m_pPlayer;

    HWND                m_hwndEvent;        // App window to receive events.    

    // Video
    BOOL                m_bHasVideo;
    float               m_fZoom;            // Zoom level (1.0 = 100%)

    // Audio
    CAudioSessionVolume             *m_pVolume;

    // etc
    float                           m_fRate;
    MFP_MEDIAITEM_CHARACTERISTICS   m_caps;


};