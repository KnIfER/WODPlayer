
#include "pch.h"
#include "resource.h"
#include "database/database_helper.h"
#include "utils/PathUtils.h"
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

void WODPlayer::SetRotate(int delta)
{
	_mMediaPlayer->SetRotation((_rotation + delta + (delta<0?360:0)) % 360);
	_rotation = _mMediaPlayer->GetRotation();
	SetPos(GetPos());
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
	//graph.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);// SmoothingMode

	//graph.FillRectangle(&brush, pControl->GetWidth()/2, 0
	//	, 8
	//	, pControl->GetHeight());

	int w = 1;
	int width = 3;
	int H = pControl->GetHeight(), W = rc.right-rc.left, max = pControl->_max;
	int height = H*2/3;
	int top = (H-height)/2;

	int height_1 = H*1/3;
	int top_1 = (H-height_1)/2;

	//graph.FillEllipse(&brush, pControl->GetWidth()/2 - w, top
	//		, width
	//		, height);

	Gdiplus::Pen pen(pControl->_trackColor);

	WODPlayer* player = (WODPlayer*)pControl->GetTag();

	int splitter_idx = 0;

	for (size_t i = 0; i < player->_bookmarks.size(); i++)
	{
		auto & bkmk = player->_bookmarks.at(i);
		auto when = bkmk.pos;
		int left = rc.left+when*1.0/max*W - w;

		if(bkmk.layer==0)
		{
			graph.FillRectangle(&brush, left, top
				, width
				, height);
			if (((splitter_idx++)%2)==0)
			{
				graph.FillRectangle(&brush, left+w, top
					, width+w
					, 2);
				graph.FillRectangle(&brush, left+w, top+height-w-w
					, width+w
					, 2);
				//graph.DrawLine(&pen, left+w, top, left+width+w+w+100, top);
			}
			else
			{
				graph.FillRectangle(&brush, left-width, top
					, width+w
					, 2);
				graph.FillRectangle(&brush, left-width, top+height-w-w
					, width+w
					, 2);
				//graph.DrawLine(&pen, left+w, top, left+width+w+w+100, top);
			}
		}
		else
		{
			graph.FillRectangle(&brush, left, top_1
				, width
				, height_1);
		}
	}
}

std::string buffer;

bool WODPlayer::PlayVideoFile(const TCHAR* path)
{
	bool ret = false;
	if (!_mMediaPlayer)
	{
		newVideoView();
	}
	//ASSERT(_mMediaPlayer);
	if (_currentPath!=path)
	{
		_durationCache = 0;
		_currentPath = path;
	}
	//xpLogTo(_currentPath);
	if (_mMediaPlayer)
	{
		ret = _mMediaPlayer->PlayVideoFile(path, _currentPath.GetData(buffer));
	}
	QkString title;
	title.Empty();
	int idx = _currentPath.ReverseFind('/'), ed = _currentPath.GetLength()-1, wenhao=-1;
	if(idx==-1) idx = _currentPath.ReverseFind('\\');
	if (_currentPath.StartWith(L"http"))
	{
		wenhao = _currentPath.ReverseFind('?');
		if(wenhao>idx+1) ed = wenhao-1;
	}
	title.Append(STR(_currentPath)+idx+1, ed-idx);
	_app->_titleBar->SetText(title);
	_app->_titleBar->SetNeedAutoCalcSize();


	idx = _currentPath.Find(L":\\");
	if(idx<0) idx = _currentPath.Find(L"://");
	if(idx<0) idx = _currentPath.GetLength()>0?1:0;
	_app->_driveTag->SetText(_currentPath.Mid(0, idx));

	idx = _currentPath.ReverseFind(L".", _currentPath.GetLength()-2);
	if(idx>0) {
		if(wenhao>idx+1) ed = wenhao-1;
		else ed = _currentPath.GetLength()-1;
		title = _currentPath.Mid(idx+1, ed-idx);
		title.MakeUpper();
		if (title==L"TS")
		{
			title = L"MPEGTS";
		}
		_app->_mimeTag->SetText(title);
	}


	_app->_titleBar->NeedParentUpdate();
	::SetWindowText(_app->GetHWND(), STR(_app->_titleBar->GetText()));



	if(!_seekbar._decorator)
	{
		_seekbar.SetTag((LONG_PTR)this);
		_seekbar._decorator = (SeekBarTrackDecorator)TimeMarksDecorator;
	}
	//if (ret)
	{
		QkString tmp;

		_timeMarked = -1;

		QkString pathBuffer;
		pathBuffer.EnsureCapacity(_currentPath.GetLength() + 30);
		pathBuffer.AsBuffer();

		if (DuiLib::PathCanonicalizeW((LPWSTR)pathBuffer.GetData(), _currentPath))
		{
			pathBuffer.RecalcSize();
			if(pathBuffer.Find('\"')) {
				pathBuffer.Replace(L"\"", L"\"\"");
			}
			int fullPathLen = pathBuffer.GetLength();
			if (DuiLib::PathRemoveFileSpecW((LPWSTR)pathBuffer.GetData()))
			{
				pathBuffer.RecalcSize();
				size_t basePathLen = pathBuffer.GetLength();
				//lxx(,STR(pathBuffer)+basePathLen+1)
				//LogIs(2, pathBuffer.GetData(threadBuffer, basePathLen+1, fullPathLen-basePathLen-1));
				_timeMarked = _app->_db->GetBookMarks(pathBuffer.GetData(threadBuffer1)
					, pathBuffer.GetData(threadBuffer, basePathLen+1, fullPathLen-basePathLen-1), _bookmarks);
			}
		}

		MarkPlaying();
		if (_mMediaPlayer->IsPaused())
		{
			_mMediaPlayer->Play();
		}
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


		QkString pathBuffer;
		pathBuffer.EnsureCapacity(_currentPath.GetLength() + 30);
		pathBuffer.AsBuffer();

		int rowId = -1;

		if (DuiLib::PathCanonicalizeW((LPWSTR)pathBuffer.GetData(), _currentPath))
		{
			pathBuffer.RecalcSize();
			if(pathBuffer.Find('\"')) {
				pathBuffer.Replace(L"\"", L"\"\"");
			}
			int fullPathLen = pathBuffer.GetLength();
			if (DuiLib::PathRemoveFileSpecW((LPWSTR)pathBuffer.GetData()))
			{
				pathBuffer.RecalcSize();
				size_t basePathLen = pathBuffer.GetLength();
				//if(basePathLen>0 && pathBuffer[basePathLen-1]=='\\')
				//{ // todo optimize
				//    basePathLen--;
				//	DuiLib::PathCanonicalizeW((LPWSTR)pathBuffer.GetData(), _currentPath);
				//	pathBuffer.SetAt(basePathLen, '\0');
				//	pathBuffer.RecalcSize();
				//}
				rowId = _app->_db->AddBookmark(pathBuffer.GetData(threadBuffer1)
					, pathBuffer.GetData(threadBuffer, basePathLen+1, fullPathLen-basePathLen-1), 0, _timeMarked, pos, duration, 0);
			}
		}

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
	__super::SetPos(rc, bNeedInvalidate);
	if(_hPlayer) {
		//::MoveWindow(_hPlayer, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, bNeedInvalidate);
		//::SetWindowPos(_hPlayer, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, bNeedInvalidate
		//lxxx('SetPos dd dd', _srcWidth, _srcHeight);
		auto hplayer = _mMediaPlayer->getHWND();
		//hplayer = GetHWND();
		int W, H;
		//if (_rotation && (_mMediaPlayer) && ((_rotation%180>=45 || _rotation%180<=135)))
		//{
		//	W = _srcHeight;
		//	H = _srcWidth;
		//}
		//else
		{
			W = _srcWidth;
			H = _srcHeight;
		}
		if(W && H) 
		{
			float width = rc.right - rc.left;
			float height = rc.bottom-rc.top;

			_minScale = min(width/W, height/H);

			float ratio = _minScale*_scale;

			int w = W*ratio;
			int h = H*ratio;

			_exRect.left = (width-w)/2;
			//_exRect.top = max(0, (height-h)/2);
			_exRect.top = (height-h)/2;

			int minY = -h+height-height/2;
			_exRect.left += _translationX;
			_exRect.top += _translationY;

			if (_scale > 1)
			{
				if (h > height)
				{
					//if(_app->_isFullScreen) minY -= _app->_bottomBar->GetHeight();
					if(_exRect.top>0) {_exRect.top=0; _translationY=-(height-h)/2;}
					else if(_exRect.top<minY) {_exRect.top=minY; _translationY=minY-(height-h)/2;}
				}
				else if (_exRect.top < 0) {_exRect.left=0; _translationX=-(width-w)/2;}
				//else if (_exRect.top > minY) {_exRect.top=minY; _translationX=minY-(height-h)/2;}
				if (w > width)
				{
					if(_exRect.left>0) {_exRect.left=0; _translationX=-(width-w)/2;}
					else if(_exRect.left<-w+width) {_exRect.left=-w+width; _translationX=-w+width-(width-w)/2;}
				}
				else if (_exRect.left < 0) {_exRect.left=0; _translationX=-(width-w)/2;}
				else if (_exRect.left > -w+width) {_exRect.left=-w+width; _translationX=-w+width-(width-w)/2;}
			}
			else {
				if (_exRect.top < minY) {_exRect.top=minY; _translationY=minY-(height-h)/2;}
			}

			_exRect.right = _exRect.left+w;
			_exRect.bottom = _exRect.top+h;
			::SetWindowPos(hplayer, HWND_TOP, 
				_exRect.left,  _exRect.top, 
				w,  h, 
				SWP_SHOWWINDOW);

			//lxxx(SetWindowPos dd dd dd dd ff, _exRect.left, _exRect.top, w, h, _scale)
		} 
		else 
		{
			::SetWindowPos(hplayer, HWND_TOP, 
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
		if(!_hPlayer)
		lxx(%ld = %ld != %ld, _hPlayer, _mMediaPlayer->getHWND(), GetHWND())
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

// doubled draw buffer for all instances
HDC         hdcMem = nullptr;
HBITMAP     hbmMem = nullptr;
HANDLE      hOld;
PAINTSTRUCT ps;
HDC         _hdc;
UINT win_width, win_height;

bool WODPlayer::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT * ret)
{
	switch (uMsg)
	{
		case MM_PREPARED:
		{
			//lxx("MPM_PREPARED max=%d %ld\n", wParam, _hPlayer);
			//lxx(MPM_PREPARED dd, _mMediaPlayer->GetDuration())
			if(!_hPlayer) {
				_hPlayer = ::GetFirstChild(_hWnd);
				_mMediaPlayer->setHWND(_hPlayer);
			}
			//LogIs(2, "MPM_PREPARED max=%d %ld\n", wParam, _hPlayer);
			_seekbar.SetMax(wParam);
			MarkPlaying(true);
			if(_mMediaPlayer)
				_mMediaPlayer->syncResolution(_srcWidth, _srcHeight);
			//LogIs(2, "syncResolution max=%d %d\n", _srcWidth, _srcHeight);
			//if(_app->_WndOp==1)
			{
				SetPos(GetPos());
			}
		} return 1;
		case MM_STOPPED:
		{
			MarkPlaying(false);
		} return 1;
		case WM_LBUTTONDOWN:
		{
			if(_app->_isFullScreen || ::GetKeyState(VK_CONTROL) < 0)
			{
				_moving = true;
				POINT pt;
				::GetCursorPos(&pt);
				_moveLastX = pt.x;
				_moveLastY = pt.y;
				::SetCapture(GetHWND());
			}
		} return 1;
		case WM_LBUTTONUP:
		{
			_moving = false;
			::ReleaseCapture();
		} return 0;
		case WM_MOUSEMOVE:
		{
			if (_moving)
			{
				POINT pt;
				::GetCursorPos(&pt);
				int X = pt.x; //LOWORD(event.lParam);
				int Y = pt.y; //HIWORD(event.lParam);
				_translationX += X-_moveLastX;
				_translationY += Y-_moveLastY;
				_moveLastX = X;
				_moveLastY = Y;
				NeedUpdate();
			}
		} return 1;
		//case WM_ERASEBKGND:
		//{
		//} return 1;
		case WM_PAINT:
		{
			//if (WS_EX_LAYERED == (WS_EX_LAYERED & GetWindowLong(hWnd, GWL_EXSTYLE))) break;;
			//RECT rcClient;
			//::GetClientRect(GetHWND(), &rcClient);

			PAINTSTRUCT ps = { 0 };
			HDC hdc = ::BeginPaint(GetHWND(), &ps);

			RECT rect = ps.rcPaint; 

			auto rcPaint = ps.rcPaint;
			//rcPaint.right = rcPaint.right - rcPaint.left;
			//rcPaint.bottom = rcPaint.bottom - rcPaint.top;
			//// Create an off-screen DC for double-buffering
			//if (!hdcMem || win_width<rcPaint.right || win_height< rcPaint.bottom)
			//{
			//	if (hdcMem)
			//	{
			//		DeleteObject(hbmMem);
			//		DeleteDC (hdcMem);
			//	}
			//	hdcMem = CreateCompatibleDC(hdc);
			//	hbmMem = CreateCompatibleBitmap(hdc, rcPaint.right, rcPaint.bottom);
			//	SelectObject(hdcMem, hbmMem);

			//	win_width = rcPaint.right;
			//	win_height = rcPaint.bottom;
			//}
			//_hdc = hdc;
			//hdc = hdcMem;

			{
				HBRUSH hbrush1 = CreateSolidBrush(~TransparentKey);
				FillRect(hdc, &_exRect, hbrush1);

				//::GetClientRect(_hPlayer, &_exRect);
				::ExcludeClipRect(hdc, _exRect.left, _exRect.top, _exRect.right, _exRect.bottom);

				HBRUSH hbrush = CreateSolidBrush(TransparentKey);
				FillRect(hdc, &rect, hbrush);

				//PAINTSTRUCT ps = { 0 };
				//::BeginPaint(GetHWND(), &ps);
				CRenderEngine::DrawColor(_hdc, ps.rcPaint, 0xFF000000);
				//::EndPaint(GetHWND(), &ps);

				// todo clean up
			}

			if (hdcMem)
			{	
				BitBlt(_hdc, rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom, hdcMem, 0, 0, SRCCOPY);
				//DeleteObject(hbmMem); DeleteDC (hdcMem); hdcMem = NULL;
			}

			::EndPaint(GetHWND(), &ps);



			//return 0;
		} return 1;
	}
	return 0;
}

void WODPlayer::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_TIMER)
	{
		if(event.wParam==0x999) {
			SetPos(m_rcItem);
			KillTimer(0x999);
		}
	}
	if (event.Type == UIEVENT_SCROLLWHEEL)
	{
		if(_app->_isFullScreen || ::GetKeyState(VK_CONTROL) < 0)
		{
			float delta = 0.25;
			//if(_scale==0)_scale = 1;
			float scale = _scale;
			int zDelta = (int) (short) HIWORD(event.wParam);
			//LogIs(2, "%d %d", event.wParam, LOWORD(event.wParam));
			if (zDelta < 0) // SB_LINEDOWN
			{
				_scale -= delta;
				if (_scale <= 1)
				{
					_bFit = true;
					_scale = 1;
					_translationX = 0;
					_translationY = 0;
				}
			}
			else
			{
				_scale += delta;
				if (_scale > 1)
				{
					_bFit = false;
				}
			}
			lxxx(scale ff dd, _scale, zDelta);
			//NeedUpdate();
			//::LockWindowUpdate(_hPlayer);
			//::SendMessage(GetHWND(), WM_SETREDRAW , FALSE, 0);

			float width = GetWidth();
			float height = GetHeight();
			_minScale = min(width/_srcWidth, height/_srcHeight);
			float ratio = _minScale*_scale;
			int w = _srcWidth*ratio;
			int h = _srcHeight*ratio;
			if (_mMediaPlayer)
			{
				::SetWindowPos(_mMediaPlayer->getHWND(), 0, 
					_exRect.left,  _exRect.top, 
					w,  h, 
					SWP_SHOWWINDOW);
			}
			//NeedUpdate();
			//SetTimer(0x999, 800, true);
			SetPos(m_rcItem);
			//::SendMessage(GetHWND(), WM_SETREDRAW , TRUE, 0);
			//::LockWindowUpdate(NULL);
		}
		return;
	}
	__super::DoEvent(event);
}
