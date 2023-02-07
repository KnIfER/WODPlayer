
#include "pch.h"
#include "resource.h"
#include "database/database_helper.h"
//#include "VideoPlayer/VLCPlayer.h"

extern VideoPlayer* initVidePlayerImpl(WODPlayer* xpp, int type);

WODPlayer::WODPlayer() {
	_timeMarked = -1;
	_handleMsg = true;
}

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

void WODPlayer::Release()
{
	if(_mMediaPlayer) {
		_mMediaPlayer->Release();
		delete _mMediaPlayer;
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
		QkString tmp;
		const char* pStr;
		if(_currentPath.Find('\"')) {
			tmp = _currentPath;
			tmp.Replace(L"\"", L"\"\"");
			pStr = tmp.GetData(threadBuffer);
		} else {
			pStr = _currentPath.GetData(threadBuffer);
		}
		_timeMarked = _app->_db->GetBookMarks(pStr, _bookmarks);
	}
	return false;
}

bool WODPlayer::AddBookmark()
{
	if(_mMediaPlayer) {
		int pos = _mMediaPlayer->GetPosition();
		int duration = _mMediaPlayer->GetDuration();
		//LogIs(2, L"%s", (LPCWSTR)_currentPath);
		_app->_db->AddBookmark(_currentPath.GetData(threadBuffer), 0, _timeMarked, pos, duration, 0);
		return true;
	}
	return false;
}

void WODPlayer::SetPos(RECT rc, bool bNeedInvalidate)
{
	__super::SetPos(rc, bNeedInvalidate);
	if(_hPlayer) {
		//::MoveWindow(_hPlayer, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, bNeedInvalidate);
		//::SetWindowPos(_hPlayer, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, bNeedInvalidate);
	

		::SetWindowPos(_mMediaPlayer->getHWND(), HWND_TOP, 
			rc.left, 
			rc.top, 
			//1*(rect.right - rect.left), 
			1*(rc.right - rc.left), 
			rc.bottom-rc.top, 
			SWP_SHOWWINDOW);
		//LogIs(2, "youre   %ld", GetHWND());
	}

	//::SendMessage(GetHWND(), MM_PREPARED, 0, 0);
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

bool WODPlayer::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& ret)
{
	switch (uMsg)
	{
		case MM_PREPARED:
		{
			//LogIs(2, "MPM_PREPARED %d\n", wParam);
			_seekbar.SetMax(wParam);
			MarkPlaying(true);
		} return 1;
		case MM_STOPPED:
		{
			MarkPlaying(false);
		} return 1;
	}
	return 0;
}