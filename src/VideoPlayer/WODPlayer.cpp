
#include "pch.h"
#include "resource.h"
#include "database/database_helper.h"
//#include "VideoPlayer/VLCPlayer.h"

extern VideoPlayer* initVidePlayerImpl(WODPlayer* xpp, const TCHAR* pluginName);

void volume_seekbar_change(SeekBar* bar, int pos) 
{
	WODPlayer* player = (WODPlayer*)bar->GetTag();
	int val = player->SetVolume(pos);
	//LogIs(2, "val = %d %d", val, pos);
}

WODPlayer::WODPlayer() {
	_timeMarked = -1;
	_handleMsg = true;

	_volumebar.SetTag((LONG_PTR)this);
	_volumebar.SetProgressAndMax(100, 100);
	_volumebar._callback = (SeekBarTrackCallback)volume_seekbar_change;
}

void WODPlayer::newVideoView()
{
	if (_mMediaPlayer)
	{
		delete _mMediaPlayer;
		_mMediaPlayer = 0;
		if(_hPlayer)
		{
			DestroyWindow(_hPlayer);
			_hPlayer = 0;
		}
	}
	QkString playerName;
	auto playerName_ = GetProfString("player");
	if(playerName_)
		playerName = playerName_->c_str();
	else 
		playerName = L"VLCExternalPlayer.dll";
	//playerName = L"MFExternalPlayer.dll";
	//playerName = L"VLCExternalPlayer.dll";
	//playerName = L"XunLeiExternalPlayer\\XunLeiExternalPlayer.dll";
	_mMediaPlayer = initVidePlayerImpl(this, playerName);
	if (_mMediaPlayer)
	{
		_hPlayer = _mMediaPlayer->getHWND();
	}
}

void WODPlayer::Stop()
{
	if(_mMediaPlayer) {
		_mMediaPlayer->Stop();
	}
}


void WODPlayer::Release()
{
	if(_hPlayer)
	{
		DestroyWindow(_hPlayer);
		_hPlayer = 0;
	}
	if(_mMediaPlayer) {
		_mMediaPlayer->Release();
		delete _mMediaPlayer;
		_mMediaPlayer = 0;
	}
}

float WODPlayer::SpeedDelta(float delta)
{
	if(delta!=0)
	{
		float speed = this->speed;
		if(delta!=0.1 && delta!=-0.1)
		{
			speed = floor(speed/delta)*delta;
		}
		speed += delta;
		if(speed==0)
		{
			speed = delta;
		}
		if(speed<0)
		{
			speed = abs(delta);
		}
		if(speed>12)
		{
			speed = 12;
		}
		this->speed = _mMediaPlayer->SetRate(speed);
		LogIs(1, "speed=%f", speed);
	}
	else
	{
		_mMediaPlayer->SetRate(speed=1);
	}
	return speed;
}

int WODPlayer::SetVolume(int volume)
{
	return _mMediaPlayer->SetVolume(volume);
}

//class DummyPlayer : public VideoPlayer
//{
//public:
//	void Play(){}
//	void Stop(){}
//	void Pause(){}
//	bool IsPlaying(){}
//	bool IsPaused(){}
//	long GetPosition(){}
//	void SetPosition(long pos){}
//	long GetDuration(){}
//	bool PlayVideoFile(const TCHAR* path){}
//};

void TimeMarksDecorator(SeekBar* pControl, Gdiplus::Graphics & graph, Gdiplus::SolidBrush & brush, RECT & rc)
{
	brush.SetColor(0xffffffff);
	graph.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);// SmoothingMode

	//graph.FillRectangle(&brush, pControl->GetWidth()/2, 0
	//	, 8
	//	, pControl->GetHeight());

	int w = 1;
	int width = 3;
	int H = pControl->GetHeight(), W = pControl->GetWidth(), max = pControl->_max;
	int height = H*2/3;
	int top = (H-height)/2;

	//graph.FillEllipse(&brush, pControl->GetWidth()/2 - w, top
	//		, width
	//		, height);

	WODPlayer* player = (WODPlayer*)pControl->GetTag();

	for (size_t i = 0; i < player->_bookmarks.size(); i++)
	{
		auto when = player->_bookmarks.at(i).pos;
		graph.FillRectangle(&brush, when*1.0/max*W - w, top
			, width
			, height);
	}
}

bool WODPlayer::PlayVideoFile(const TCHAR* path)
{
	bool ret = false;
	if (!_mMediaPlayer)
	{
		newVideoView();
	}
	//ASSERT(_mMediaPlayer);
	if (_mMediaPlayer)
	{
		ret = _mMediaPlayer->PlayVideoFile(path);
	}
	_currentPath = path;
	if(!_seekbar._decorator)
	{
		_seekbar.SetTag((LONG_PTR)this);
		_seekbar._decorator = (SeekBarTrackDecorator)TimeMarksDecorator;
	}
	//if (ret)
	{
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
		MarkPlaying();
		return true;
	}
	return false;
}

bool WODPlayer::AddBookmark()
{
	if(_mMediaPlayer) { // todo use binary insert
		int pos = _mMediaPlayer->GetPosition();
		int duration = _mMediaPlayer->GetDuration();
		//LogIs(2, L"%s", (LPCWSTR)_currentPath);
		auto rowId = _app->_db->AddBookmark(_currentPath.GetData(threadBuffer), 0, _timeMarked, pos, duration, 0);

		int idx = -1;
		for (size_t i = 0; i < _bookmarks.size(); i++)
		{
			if(_bookmarks[i].pos>=pos) {
				if(_bookmarks[i].pos==pos) return false;
				idx = i;
				break;
			}
		}
		if(idx>=0)
		{
			_selectedBookmark = idx;
			_bookmarks.insert(_bookmarks.begin()+idx, {pos, rowId});
		}
		else
		{
			_selectedBookmark = _bookmarks.size();
			_bookmarks.push_back({pos, rowId});
		}
		_seekbar.Invalidate();
		return true;
	}
	return false;
}

void WODPlayer::SelectBookMark(int index)
{
	if (index>=0 && index<_bookmarks.size())
	{
		_mMediaPlayer->SetPosition(_bookmarks[index].pos);
		_selectedBookmark = index;
		if (!_isPlaying)
		{
			_seekbar.SetProgress(_bookmarks[index].pos);
		}
	}
}

void WODPlayer::DelBookmark(int index)
{
	if (index>=0 && index<_bookmarks.size())
	{
		const auto & bkmk =  _bookmarks[index];
		_app->_db->DelBookmark(bkmk.rowID);
		_bookmarks.erase(_bookmarks.begin()+index);
		_seekbar.Invalidate();
		_selectedBookmark = -1;
	}
}

void WODPlayer::SetPos(RECT rc, bool bNeedInvalidate)
{
	if(_hPlayer) {
		//::MoveWindow(_hPlayer, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, bNeedInvalidate);
		//::SetWindowPos(_hPlayer, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, bNeedInvalidate);

		if(_srcWidth && _srcHeight) 
		{
			float width = rc.right - rc.left;
			float height = rc.bottom-rc.top;

			float ratio = min(width/_srcWidth, height/_srcHeight)*_scale;

			int w = _srcWidth*ratio;
			int h = _srcHeight*ratio;

			_exRect.left = (width-w)/2;
			//_exRect.top = max(0, (height-h)/2);
			_exRect.top = (height-h)/2;
			_exRect.right = _exRect.left+w;
			_exRect.bottom = _exRect.top+h;
			::SetWindowPos(_mMediaPlayer->getHWND(), HWND_TOP, 
				_exRect.left,  _exRect.top, 
				w,  h, 
				SWP_SHOWWINDOW);
		} 
		else 
		{
			::SetWindowPos(_mMediaPlayer->getHWND(), HWND_TOP, 
				rc.left, 
				rc.top, 
				//1*(rect.right - rect.left), 
				1*(rc.right - rc.left), 
				rc.bottom-rc.top, 
				SWP_SHOWWINDOW);
		}

		// 
		//LogIs(2, "youre   %ld", GetHWND());
	}
	__super::SetPos(rc, bNeedInvalidate);

	//::SendMessage(GetHWND(), MM_PREPARED, 0, 0);
}

bool WODPlayer::IsMediaPlayerWindow(HWND hwnd)
{
	return hwnd==_hWnd||IsChild(_hWnd, hwnd);
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
	}
	_app->MarkPlaying(playing);
}

bool WODPlayer::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& ret)
{
	switch (uMsg)
	{
		case MM_PREPARED:
		{
			//LogIs(2, "MPM_PREPARED max=%d\n", wParam);
			_seekbar.SetMax(wParam);
			MarkPlaying(true);
			if(_mMediaPlayer)
				_mMediaPlayer->syncResolution(_srcWidth, _srcHeight);
			//if(_app->_WndOp==1)
			{
				SetPos(GetPos());
			}
		} return 1;
		case MM_STOPPED:
		{
			MarkPlaying(false);
		} return 1;
		case WM_PAINT:
		{
			//if (WS_EX_LAYERED == (WS_EX_LAYERED & GetWindowLong(hWnd, GWL_EXSTYLE))) break;;
			//RECT rcClient;
			//::GetClientRect(GetHWND(), &rcClient);

			PAINTSTRUCT ps = { 0 };
			HDC hdc = ::BeginPaint(GetHWND(), &ps);

			HBRUSH hbrush1 = CreateSolidBrush(~TransparentKey);
			FillRect(hdc, &_exRect, hbrush1);

			//::GetClientRect(_hPlayer, &_exRect);
			::ExcludeClipRect(hdc, _exRect.left, _exRect.top, _exRect.right, _exRect.bottom);
			//::ExcludeClipRect(hdc, 0, 0, 100, 100);

			RECT rect = ps.rcPaint; 
			//rect.bottom -= 50;

			//rect.right = rect.left+(rect.right-rect.left)/2;

			HBRUSH hbrush = CreateSolidBrush(TransparentKey);

			FillRect(hdc, &rect, hbrush);



			::EndPaint(GetHWND(), &ps);


			//PAINTSTRUCT ps = { 0 };
			//::BeginPaint(GetHWND(), &ps);
			//CRenderEngine::DrawColor(, ps.rcPaint, 0xFF000000);
			//::EndPaint(GetHWND(), &ps);

			// todo clean up

			return 0;
		} return 1;
	}
	return 0;
}