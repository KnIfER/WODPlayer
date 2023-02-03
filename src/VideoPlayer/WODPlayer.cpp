
#include "pch.h"
#include "resource.h"
//#include "VideoPlayer/VLCPlayer.h"

extern VideoPlayer* initVidePlayerImpl(WODPlayer* xpp, int type);


void WODPlayer::newVideoView()
{
	if (_mMediaPlayer)
	{
		delete _mMediaPlayer;
		_mMediaPlayer = 0;
	}
	_mMediaPlayer = initVidePlayerImpl(this, 1);
	if (_mMediaPlayer)
	{
		_hPlayer = _mMediaPlayer->getHWND();
	}
}

bool WODPlayer::PlayVideoFile(TCHAR* path)
{
	bool ret = false;
	if (!_mMediaPlayer)
	{
		newVideoView();
	}
	if (_mMediaPlayer)
	{
		ret = _mMediaPlayer->PlayVideoFile(path);
	}
	if (ret)
	{
		_currentPath = path;
	}
	return false;
}

void WODPlayer::SetPos(RECT rc, bool bNeedInvalidate)
{
	__super::SetPos(rc, bNeedInvalidate);
	if(_hPlayer) {
		//::MoveWindow(_hPlayer, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, bNeedInvalidate);
		//::SetWindowPos(_hPlayer, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, bNeedInvalidate);
	}

	::SetWindowPos(_mMediaPlayer->getHWND(), HWND_TOP, 
		rc.left, 
		rc.top, 
		//1*(rect.right - rect.left), 
		1*(rc.right - rc.left), 
		rc.bottom-rc.top, 
		SWP_SHOWWINDOW);
}

bool WODPlayer::IsMediaPlayerWindow(HWND hwnd)
{
	return hwnd==_hPlayer||IsChild(_hPlayer, hwnd);
}

HWND WODPlayer::GetMediaPlayerHWND()
{
	return _hPlayer;
}

void WODPlayer::Toggle()
{
	if (_mMediaPlayer)
	{
		if (_isPlaying)
		{
			_mMediaPlayer->Pause();
		}
		else 
		{
			_mMediaPlayer->Play();
		}
		MarkPlaying(!_isPlaying);
	}
}

void WODPlayer::MarkPlaying(bool playing)
{
	if (_isPlaying!=playing)
	{
		_isPlaying = playing;
		_app->MarkPlaying(playing);
	}
}
