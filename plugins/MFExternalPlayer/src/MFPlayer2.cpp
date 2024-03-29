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

#include "MFPlayer.h"
#include "MFPlayer2.h"


#define kClassWindow L"VideoFrame1"

LRESULT CALLBACK MF_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL MF_regWndClassWin(LPCTSTR lpcsClassName, DWORD dwStyle)
{
    WNDCLASS wndclass = { 0 };

    wndclass.style = dwStyle;
    wndclass.lpfnWndProc = MF_WndProc;
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

//-----------------------------------------------------------------------------
// CreateInstance
//
// Creates an instance of the MFPlayer2 object.
//-----------------------------------------------------------------------------

MFPlayer2::MFPlayer2(int & error_code, HINSTANCE hInstance, HWND hParent) : 
    m_cRef(1), 
    m_pPlayer(NULL),
    m_hwndEvent(hParent),
    m_bHasVideo(FALSE),
    m_fZoom(1.0f),
    m_pVolume(NULL),
    m_fRate(1.0f),
    m_caps(0)
{
    HRESULT hr = S_OK;
    error_code = 0;
    // E_OUTOFMEMORY

    //------------------------------------------------------------------------------
    //  Initialize
    //  Creates an instance of the MFPlay player object.
    //
    //  hwndVideo: 
    //  Handle to the video window.
    //------------------------------------------------------------------------------

    SafeRelease(&m_pPlayer);


    MF_regWndClassWin(kClassWindow, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS);
    HWND hWnd = ::CreateWindowEx(WS_EX_TOOLWINDOW , kClassWindow , NULL
        , WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS
        , 0 , 0 , 840 , 680 , hParent , NULL , ::GetModuleHandle(NULL), NULL);
    if (hWnd)
    {
        _hWnd=hWnd;
    }
    init(hInstance, hParent);

    hr = MFPCreateMediaPlayer(
        NULL,
        FALSE,          // Start playback automatically?
        0,              // Flags
        this,           // Callback pointer   
        _hWnd,      // Video window
        &m_pPlayer
    );

    if (SUCCEEDED(hr)) 
    { 
        // Also create the object that manages to audio session.
        // This can fail if the machine does not have an audio end-point.

        HRESULT hrAudio = CAudioSessionVolume::CreateInstance(
            WM_AUDIO_EVENT, 
            m_hwndEvent, 
            &m_pVolume
        );

        if (SUCCEEDED(hrAudio))
        {
            // Ask for audio session events.
            hrAudio = m_pVolume->EnableNotifications(TRUE);
        }

        if (FAILED(hrAudio))
        {
            SafeRelease(&m_pVolume);
        }
        //

        if (SUCCEEDED(hr))
        {
            error_code = 0;
            AddRef();
        }

        //error_code = -1;
        //SafeRelease(&pPlayer);
        //return;
    }

    //m_pPlayer->SetAspectRatioMode(MFVideoARMode_None);
}


//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

MFPlayer2::~MFPlayer2()
{
    if (m_pVolume)
    {
        (void)m_pVolume->EnableNotifications(FALSE);
    }
    LogIs("MFPlayer2::~MFPlayer2 解构");
    SafeRelease(&m_pPlayer);
    SafeRelease(&m_pVolume);
}

//***************************** IUnknown methods *****************************//


//------------------------------------------------------------------------------
//  AddRef
//------------------------------------------------------------------------------

ULONG MFPlayer2::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}


//------------------------------------------------------------------------------
//  Release
//------------------------------------------------------------------------------

ULONG MFPlayer2::Release()
{
    ULONG uCount = InterlockedDecrement(&m_cRef);
    if (uCount == 0)
    {
        delete this;
    }
    return uCount;
}

void MFPlayer2::DoRelease()
{
    Stop();
    Shutdown();
    Release();
}


//------------------------------------------------------------------------------
//  QueryInterface
//------------------------------------------------------------------------------

STDMETHODIMP MFPlayer2::QueryInterface(REFIID riid, void** ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT(MFPlayer2, IMFPMediaPlayerCallback),
        { 0 },
    };
    return QISearch(this, qit, riid, ppv);
}


//********************* IMFPMediaPlayerCallback methods **********************//

//-----------------------------------------------------------------------------
// OnMediaPlayerEvent
//
// Notifies the object of an MFPlay event.
//-----------------------------------------------------------------------------


#define MM_PREPARED              (WM_USER)
#define MM_STOPPED               (WM_USER+1)

void MFPlayer2::OnMediaPlayerEvent(MFP_EVENT_HEADER * pEventHeader)
{
    if (FAILED(pEventHeader->hrEvent))
    {
        NotifyError(pEventHeader->hrEvent);
        return;
    }

    switch (pEventHeader->eEventType)
    {
    case MFP_EVENT_TYPE_MEDIAITEM_CREATED:
        OnMediaItemCreated(MFP_GET_MEDIAITEM_CREATED_EVENT(pEventHeader));
        break;

    case MFP_EVENT_TYPE_MEDIAITEM_SET:
        OnMediaItemSet(MFP_GET_MEDIAITEM_SET_EVENT(pEventHeader));
        break;

    case MFP_EVENT_TYPE_RATE_SET:
        OnRateSet(MFP_GET_RATE_SET_EVENT(pEventHeader));
        break;

    case MFP_EVENT_TYPE_PLAY:
        PostMessage(getHParent(), MM_PREPARED, GetDuration()/10000, 0);
        break;

    case MFP_EVENT_TYPE_PLAYBACK_ENDED:
        Play();
        break;
    case MFP_EVENT_TYPE_STOP:
        SetPlaybackRate(1.0f);
        PostMessage(getHParent(), MM_STOPPED, GetDuration(), 0);
        break;

    }

    NotifyState(pEventHeader->eState);
}


//*************************** Other class methods ****************************//

//-----------------------------------------------------------------------------
// OpenURL
// 
// Open a media file by URL.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::OpenURL(const WCHAR *sURL)
{
    HRESULT hr = S_OK;

    if (sURL == NULL)
    {
        return E_POINTER;
    }

    if (m_pPlayer == NULL)
    {
        return E_UNEXPECTED;
    }

    // Create a new media item for this URL.
    hr = m_pPlayer->CreateMediaItemFromURL(sURL, FALSE, 0, NULL);
    m_pPlayer->SetAspectRatioMode(MFVideoARMode_None);
    //m_pPlayer->SetAspectRatioMode(MFVideoARMode_None);

    // The CreateMediaItemFromURL method completes asynchronously. When it does,
    // MFPlay sends an MFP_EVENT_TYPE_MEDIAITEM_CREATED event.

    return hr;
}


//-----------------------------------------------------------------------------
// Play
//
// Start playback.
//-----------------------------------------------------------------------------

void MFPlayer2::Play()
{
    HRESULT hr = S_OK;

    if (m_pPlayer)
    {
        hr = m_pPlayer->Play();
    }
    //return hr;
}


//-----------------------------------------------------------------------------
// Pause
//
// Pause playback.
//-----------------------------------------------------------------------------

void MFPlayer2::Pause()
{
    HRESULT hr = S_OK;

    if (m_pPlayer)
    {
        hr = m_pPlayer->Pause();
    }
    //return hr;
}

void MFPlayer2::Stop()
{
    if (m_pPlayer)
    {
        m_pPlayer->Stop();
    }
}

bool MFPlayer2::IsPlaying()
{
    MFP_MEDIAPLAYER_STATE state;
    GetState(&state);
    return state==MFP_MEDIAPLAYER_STATE_PLAYING;
}

bool MFPlayer2::IsPaused()
{
    MFP_MEDIAPLAYER_STATE state;
    GetState(&state);
    return state==MFP_MEDIAPLAYER_STATE_PAUSED;
}

long MFPlayer2::GetPosition()
{
    MFTIME value=0;
    GetCurrentPosition(&value);
    return value;
}

void MFPlayer2::SetPosition(long pos)
{
    SetPosition((MFTIME)pos);
}

long MFPlayer2::GetDuration()
{
    MFTIME value=0;
    GetDuration(&value);
    return value;
}

bool MFPlayer2::PlayVideoFile(const TCHAR* path)
{
    return SUCCEEDED(OpenURL(path));
}


//-----------------------------------------------------------------------------
// Shutdown
//
// Shutdown the MFPlay object.
//-----------------------------------------------------------------------------

void MFPlayer2::SyncSize(unsigned int* x, unsigned int* y)
{
    if (m_pPlayer)
    {
        SIZE szVideo;
        //MFVideoNormalizedRect rect;
        //m_pPlayer->GetVideoSourceRect(&rect);
        m_pPlayer->GetNativeVideoSize(&szVideo, 0);
        m_pPlayer->SetAspectRatioMode(MFVideoARMode_None);
        //*x = rect.right - rect.left;
        //*y = rect.bottom - rect.top;
        *x = szVideo.cx;
        *y = szVideo.cy;
        //LogIs("GetVideoSourceRect %d %d", x, y);
    }
}

HRESULT MFPlayer2::Shutdown()
{
    HRESULT hr = S_OK;

    if (m_pPlayer)
    {
        hr = m_pPlayer->Shutdown();
    }

    return hr;
}


//-----------------------------------------------------------------------------
// GetState
//
// Get the current state of the MFPlay object.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::GetState(MFP_MEDIAPLAYER_STATE *pState)
{
    if (pState == NULL)
    {
        return E_POINTER;
    }

    HRESULT hr = S_OK;
    MFP_MEDIAPLAYER_STATE state = MFP_MEDIAPLAYER_STATE_EMPTY;

    if (m_pPlayer)
    {
        hr = m_pPlayer->GetState(&state);
    }

    if (SUCCEEDED(hr))
    {
        *pState = state;
    }

    return hr;
}


//-----------------------------------------------------------------------------
// GetMetadata
//
// Get a read-only property store that contains metadata for the current
// media file.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::GetMetadata(IPropertyStore **ppProp)
{
    if (ppProp == NULL)
    {
        return E_POINTER;
    }
    if (m_pPlayer == NULL)
    {
        return E_FAIL;
    }

    HRESULT hr = S_OK;

    IMFPMediaItem *pItem = NULL;
    IPropertyStore *pProp = NULL;

    hr = m_pPlayer->GetMediaItem(&pItem);
    if (FAILED(hr)) { goto done; }

    hr = pItem->GetMetadata(&pProp);
    if (FAILED(hr)) { goto done; }

    *ppProp = pProp;
    (*ppProp)->AddRef();

done:
    SafeRelease(&pProp);
    SafeRelease(&pItem);
    return hr;
}



//-----------------------------------------------------------------------------
// HasVideo
//
// Queries if current media file has a video stream.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::HasVideo(BOOL *pfHasVideo)
{
    if (pfHasVideo == NULL)
    {
        return E_POINTER;
    }

    *pfHasVideo = m_bHasVideo;

    return S_OK;
}



//-----------------------------------------------------------------------------
// SetZoom
//
// Sets the video zoom level. 
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::SetZoom(float fZoom)
{
    if (fZoom < 1.0f)
    {
        return E_INVALIDARG;
    }

    if (m_fZoom == fZoom)
    {
        return S_OK; // no-op
    }

    m_fZoom = fZoom;

    return SetZoom();
}


//-----------------------------------------------------------------------------
// SetZoom
//
// Sets the video zoom level (using the current cached zoom value).
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::SetZoom()
{
    HRESULT hr = S_OK;

    if (m_bHasVideo && m_pPlayer)
    {
        if (m_fZoom == 1.0f)
        {
            // For 100% zoom, the normalized rectangle is {0,0,1,1}.

            MFVideoNormalizedRect nrcSource = { 0.0f, 0.0f, 1.0f, 1.0f };

            hr = m_pPlayer->SetVideoSourceRect(&nrcSource);
        }
        else
        {
            // For higher zoom levels, calculate the normalized rectangle.

            float fMargin = (0.5f - (0.5f / m_fZoom));

            MFVideoNormalizedRect nrcSource = { 
                fMargin, fMargin, (1.0f - fMargin), (1.0f - fMargin)
            };

            hr = m_pPlayer->SetVideoSourceRect(&nrcSource);
        }
    }
    return hr;
}


//-----------------------------------------------------------------------------
// UpdateVideo
// 
// Call this method to repaint the current video frame or when the video
// window is resized.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::UpdateVideo()
{
    HRESULT hr = S_OK;

    if (m_pPlayer)
    {
        hr = m_pPlayer->UpdateVideo();
    }

    return hr;
}


//-----------------------------------------------------------------------------
// SetVolume
//
// Sets the audio volume.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::SetVolume(float fLevel)
{
    HRESULT hr = S_OK;
    if (m_pVolume)
    {
        hr = m_pVolume->SetVolume(fLevel);
    }
    else
    {
        hr = E_FAIL;
    }

    return hr;
}



//-----------------------------------------------------------------------------
// GetVolume
//
// Gets the audio volume.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::GetVolume(float *pfLevel)
{
    HRESULT hr = S_OK;

    if (pfLevel == NULL)
    {
        return E_POINTER;
    }

    if (m_pVolume)
    {
        hr = m_pVolume->GetVolume(pfLevel);
    }
    else
    {
        hr = E_FAIL;
    }

    return hr;
}


//-----------------------------------------------------------------------------
// SetMute
//
// Mutes or unmutes the audio.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::SetMute(BOOL bMute)
{
    HRESULT hr = S_OK;

    if (m_pVolume)
    {
        hr = m_pVolume->SetMute(bMute);
    }
    else
    {
        hr = E_FAIL;
    }

    return hr;
}


//-----------------------------------------------------------------------------
// GetMute
//
// Gets the audio mute state.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::GetMute(BOOL *pbMute)
{
    HRESULT hr = S_OK;

    if (pbMute == NULL)
    {
        return E_POINTER;
    }

    if (m_pVolume)
    {
        hr = m_pVolume->GetMute(pbMute);
    }
    else
    {
        hr = E_FAIL;
    }

    return hr;
}


//-----------------------------------------------------------------------------
// CanSeek
//
// Queries whether the current media file is seekable.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::CanSeek(BOOL *pbCanSeek)
{
    *pbCanSeek = ((m_caps & MFP_MEDIAITEM_CAN_SEEK) && !(m_caps  & MFP_MEDIAITEM_HAS_SLOW_SEEK));

    return S_OK;
}


//-----------------------------------------------------------------------------
// GetDuration
//
// Gets the playback duration.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::GetDuration(MFTIME *phnsDuration)
{
    HRESULT hr = E_FAIL;

    PROPVARIANT var;
    PropVariantInit(&var);

    if (m_pPlayer)
    {
        hr = m_pPlayer->GetDuration(
            MFP_POSITIONTYPE_100NS,
            &var
            );

        if (SUCCEEDED(hr))
        {
            *phnsDuration = var.uhVal.QuadPart;
        }
    }

    PropVariantClear(&var);
    return hr;
}


//-----------------------------------------------------------------------------
// GetCurrentPosition
// 
// Gets the current playback position.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::GetCurrentPosition(MFTIME *phnsPosition)
{
    HRESULT hr = E_FAIL;

    PROPVARIANT var;
    PropVariantInit(&var);

    if (m_pPlayer)
    {
        hr = m_pPlayer->GetPosition(
            MFP_POSITIONTYPE_100NS,
            &var
            );

        if (SUCCEEDED(hr))
        {
            *phnsPosition = var.hVal.QuadPart;
        }
    }

    PropVariantClear(&var);
    return hr;
}


//-----------------------------------------------------------------------------
// SetPosition
//
// Sets the current playback position.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::SetPosition(MFTIME hnsPosition)
{
    HRESULT hr = E_FAIL;

    PROPVARIANT var;
    PropVariantInit(&var);

    if (m_pPlayer)
    {
        var.vt = VT_I8;
        var.hVal.QuadPart = hnsPosition;

        hr = m_pPlayer->SetPosition(
            MFP_POSITIONTYPE_100NS,
            &var
            );
    }

    PropVariantClear(&var);
    return hr;
}


//-----------------------------------------------------------------------------
// CanFastForward
//
// Queries whether the current media file supports fast-forward playback.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::CanFastForward(BOOL *pbCanFF)
{
    HRESULT hr = S_OK;
    BOOL bCanFF = FALSE;

    float fSlowest = 0.0f, fFastest = 0.0f;

    if (m_pPlayer)
    {
        // Ask for the supported rates in the forward direction.
        hr = m_pPlayer->GetSupportedRates(TRUE, &fSlowest, &fFastest);
    }

    if (SUCCEEDED(hr) && fFastest > 1.0f)
    {
        bCanFF = TRUE;
    }

    *pbCanFF = bCanFF;
    return S_OK;
}


//-----------------------------------------------------------------------------
// CanRewind
//
// Queries whether the current media file supports reverse playback.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::CanRewind(BOOL *pbCanRewind)
{
    HRESULT hr = S_OK;
    BOOL bCanRew = FALSE;

    float fSlowest = 0.0f, fFastest = 0.0f;

    if (m_pPlayer)
    {
        // Ask for the supported rates in the reverse direction.
        hr = m_pPlayer->GetSupportedRates(FALSE, &fSlowest, &fFastest);
    }

    if (SUCCEEDED(hr))
    {
        bCanRew = TRUE;
    }

    *pbCanRewind = bCanRew;
    return S_OK;
}


//-----------------------------------------------------------------------------
// SetPlaybackRate
//
// Sets the playback rate.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::SetPlaybackRate(float fRate)
{
    if (fRate == GetNominalRate())
    {
        return S_OK; // no-op
    }

    HRESULT hr = S_OK;
    
    hr = m_pPlayer->SetRate(fRate);

    if (SUCCEEDED(hr))
    {
        m_fRate = fRate;
    }

    return hr;
}

float MFPlayer2::SetRate(float fRate)
{
    if(fRate!=0)
    {
        if (fRate == GetNominalRate())
        {
            return fRate;
        }

        if (SUCCEEDED(m_pPlayer->SetRate(fRate)))
        {
            m_fRate = fRate;
            return m_fRate;
        }
    }
    m_pPlayer->GetRate(&fRate);
    return fRate;
}


//------------------------------------------------------------------------------
// FastForward
//
// Switches to fast-forward playback, as follows:
// - If the current rate is < 0 (reverse play), switch to 1x speed.
// - Otherwise, double the current playback rate.
//
// This method is just for convenience; the app could call SetPlaybackRate.
//------------------------------------------------------------------------------

HRESULT MFPlayer2::FastForward()
{
    HRESULT hr = S_OK;
    float   fTarget = GetNominalRate() * 2;

    if (fTarget <= 0.0f)
    {
        fTarget = 1.0f;
    }

    hr = SetPlaybackRate(fTarget);

    return hr;
}

//------------------------------------------------------------------------------
// FastForward
//
// Switches to reverse playback, as follows:
// - If the current rate is > 0 (forward playback), switch to -1x speed.
// - Otherwise, double the current (reverse) playback rate.
//
// This method is for convenience; the app could call SetPlaybackRate.
//------------------------------------------------------------------------------

HRESULT MFPlayer2::Rewind()
{
    HRESULT hr = S_OK;
    float   fTarget = GetNominalRate() * 2;

    if (fTarget >= 0.0f)
    {   
        fTarget = -1.0f;
    }

    hr = SetPlaybackRate(fTarget);

    return hr;
}


//-----------------------------------------------------------------------------
// FrameStep
//
// Steps forward one frame.
//-----------------------------------------------------------------------------

HRESULT MFPlayer2::FrameStep()
{
    HRESULT hr = E_FAIL;

    if (m_pPlayer)
    {
        hr = m_pPlayer->FrameStep();
    }

    return hr;
}


//-----------------------------------------------------------------------------
// SetEffect
//
// Adds a video or audio effect.
//
// To remove all effects, pass NULL.
//-----------------------------------------------------------------------------

HRESULT  MFPlayer2::SetEffect(IMFTransform *pMFT)
{
    if (m_pPlayer == NULL)
    {
        return E_FAIL;
    }

    HRESULT hr = S_OK;

    if (pMFT == NULL)
    {
        hr = m_pPlayer->RemoveAllEffects();
    }
    else
    {
        hr = m_pPlayer->InsertEffect(pMFT, TRUE);
    }

    return hr;
}


// MFPlay event handler functions.



//-----------------------------------------------------------------------------
// OnMediaItemCreated
//
// Called when the CreateMediaItem method completes.
//-----------------------------------------------------------------------------

void MFPlayer2::OnMediaItemCreated(MFP_MEDIAITEM_CREATED_EVENT *pEvent)
{
    HRESULT hr = S_OK; 

    IUnknown *pMFT = NULL;

    m_bHasVideo = TRUE;

    if ((m_pPlayer != NULL) && (pEvent->pMediaItem != NULL))
    {
        BOOL bHasVideo = FALSE, bIsSelected = FALSE;

        hr = pEvent->pMediaItem->HasVideo(&bHasVideo, &bIsSelected);
        if (FAILED(hr)) { goto done; }

        m_bHasVideo = bHasVideo && bIsSelected;

        hr = m_pPlayer->SetMediaItem(pEvent->pMediaItem);
        if (FAILED(hr)) { goto done; }
    }

done:
    if (FAILED(hr))
    {
        NotifyError(hr);
    }
    SafeRelease(&pMFT);
}

//-----------------------------------------------------------------------------
// OnMediaItemSet
//
// Called when the SetMediaItem method completes.
//-----------------------------------------------------------------------------

void MFPlayer2::OnMediaItemSet(MFP_MEDIAITEM_SET_EVENT *pEvent)
{
    HRESULT hr = S_OK;

    hr = pEvent->header.hrEvent;
    if (FAILED(hr)) { goto done; }

    if (pEvent->pMediaItem)
    {
        hr = pEvent->pMediaItem->GetCharacteristics(&m_caps);
        if (FAILED(hr)) { goto done; }
    }

    hr = m_pPlayer->Play();

done:
    if (FAILED(hr))
    {
        NotifyError(hr);
    }
}



//-----------------------------------------------------------------------------
// OnRateSet
//
// Called when the SetRate method completes.
//-----------------------------------------------------------------------------

void MFPlayer2::OnRateSet(MFP_RATE_SET_EVENT *pEvent)
{
    m_fRate = pEvent->flRate;
}