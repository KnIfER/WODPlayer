
#include "pch.h"
#include "resource.h"
#include "database/database_helper.h"
#include "utils/PathUtils.h"
//#include "VideoPlayer/VLCPlayer.h"

extern VideoPlayer* initVidePlayerImpl(WODPlayer* xpp, const TCHAR* pluginName, bool isMain);
extern long _bakTime;

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
	auto playerName_ = GetProfString(isMain?"player":"player");
	if(playerName_)
		playerName = playerName_->c_str();
	else 
		playerName = isMain?L"VLCExternalPlayer.dll":L"MFExternalPlayer.dll";
	//playerName = L"MFExternalPlayer.dll";
	//playerName = L"VLCExternalPlayer.dll";
	//playerName = L"XunLeiExternalPlayer\\XunLeiExternalPlayer.dll";
	_mMediaPlayer = initVidePlayerImpl(this, playerName, isMain);
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
	bStopped = 1;
}


void WODPlayer::Release()
{
	if(_hPlayer)
	{
		DestroyWindow(_hPlayer);
		_hPlayer = 0;
	}
	if(_mMediaPlayer) {
		//_mMediaPlayer->Release();
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
		LogIs(1, "speed=%f", speed); // todo 回显
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

class DummyPlayer : public VideoPlayer
{
public:
	void Play(){}
	void Stop(){}
	void Pause(){}
	bool IsPlaying(){return 0;}
	bool IsPaused(){return 0;}
	long GetPosition(){return 0;}
	virtual void SetPosition(long pos, bool fastSeek){};
	virtual void SetLoop(bool loop){};
	void SetPosition(long pos){}
	long GetDuration(){return 999;}
	bool PlayVideoFile(const TCHAR* path, const CHAR* path1){return 0;}
	float SetRate(float rate){return 1;};
	int SetVolume(int volume){return 999;};
};

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

	//BOOL zoomedIn = true;
	//int zoomedInBy = 15*60*1000/2;
	//LONG zoomedInAt = pControl->_progress;
	//int zoomInWeight = 80;

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

void TimeMarksFloatDecorator(SeekBar* pControl, Gdiplus::Graphics & graph, Gdiplus::SolidBrush & brush, RECT & rc)
{
	WODPlayer* player = (WODPlayer*)pControl->GetTag();
	auto & seekbar = player->_seekbar;
	long pos = seekbar._progress;
	long duration = seekbar._max;
	long sub_duration = player->seekfloat_duration;
	long sub_pos = pos % (sub_duration);
	int lunhui = pos / sub_duration;

	long timeRangeMin = lunhui*sub_duration;
	long timeRangeMax = lunhui*sub_duration + sub_duration;

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

	int left = pControl->GetFixedXY().cx;

	brush.SetColor(0xff958700);
	//brush.SetColor(0xff000000);
	// 画原始进度
	//graph.FillEllipse(&brush, seekbar.GetPos().left+(int)(pos*1.f/duration*seekbar.GetWidth()-w), 0
	//	, width*2
	//	, H);

	int thumbSz = seekbar._thumbSize;
	int radius = thumbSz/2;
	int ttop = pControl->GetHeight()/2 - radius;

	auto mode = graph.GetSmoothingMode();
	graph.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);// SmoothingMode
	graph.FillPie(&brush, (INT)seekbar.GetPos().left+(int)(pos*1.f/duration*seekbar.GetWidth()), ttop, thumbSz, thumbSz, 0, 360);
	graph.SetSmoothingMode(mode);

	brush.SetColor(0xffffffff);
	//graph.FillEllipse(&brush, left + pControl->GetWidth()/2 - w, top
	//		, width
	//		, height);

	Gdiplus::Pen pen(pControl->_trackColor);


	int splitter_idx = 0;

	//BOOL zoomedIn = true;
	//int zoomedInBy = 15*60*1000/2;
	//LONG zoomedInAt = pControl->_progress;
	//int zoomInWeight = 80;

	for (size_t i = 0; i < player->_bookmarks.size(); i++)
	{
		auto & bkmk = player->_bookmarks.at(i);
		auto when = bkmk.pos;
		if(when>=timeRangeMin && when<=timeRangeMax) {
			when = (when-timeRangeMin);
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
		else if(bkmk.layer==0)
			splitter_idx++;
	}
}

std::string buffer;
bool hasTrack = false;

void seekchange(SeekBar* bar, int pos) {
	WODPlayer* player = (WODPlayer*)bar->GetTag();
	player->SetPosition(pos, true);
	if(player->isMain && hasTrack) 
	{
		player->_app->_audioPlayer.SetPosition(pos, true);
	}
}

void seekchangefloat(SeekBar* bar, int posf) {
	WODPlayer* player = (WODPlayer*)bar->GetTag();
	auto & seekbar = player->_seekbar;
	long pos = seekbar._progress;
	long duration = seekbar._max;
	long sub_duration = player->seekfloat_duration;
	long sub_pos = pos % (sub_duration);
	int lunhui = pos / sub_duration;

	player->SetPosition(lunhui*sub_duration + posf, true);
}

extern CControlUI* _timeLabel;
extern CControlUI* _durationLabel;

#include <iostream>
#include <string>

int parsint(const TCHAR* s, int i, int end, int val) {
	//lxx(digit dd dd dd, i, end, val)
	//if (o == nullptr) return val;

	//std::string s = std::to_string(*static_cast<int*>(o));
	//int i = 0, len = s.length();
	int len = end-i;
	if (len <= 0) return val;

	int result = 0;
	bool negative = false;
	int limit = -INT_MAX;
	int multmin;
	int digit;

	if (s[i] == '-') {
		negative = true;
		limit = INT_MIN;
		i++;
	}

	multmin = limit / 10;

	while (i < end) {
		digit = s[i++] - L'0';

		if (digit < 0 || digit>9 || result < multmin || (result *= 10) < limit + digit) {
			if (i > (negative ? 1 : 0)) break;
			else return val;
		}

		result -= digit;
	}

	return negative ? result : -result;
}


// Function to check if a number is prime
bool isPrime(int number) {
	if (number <= 1) return false; // 0 and 1 are not prime numbers
	if (number <= 3) return true; // 2 and 3 are prime numbers

								  // Check from 2 to square root of the number
	for (int i = 2; i * i <= number; i++) {
		if (number % i == 0) return false;
	}
	return true;
}

QkString voals = L"aeiouAEIOU";

bool isPunct(TCHAR ch) {
	switch (ch) {
	case L' ':
	case L'-':
	case L'.':
	case L'[':
	case L']':
	case L'_':
		return true;
	}
	return false;
}
static void decryptFileName(QkString & fileName) {
	// 
	int idx = -1;
	if(fileName.EndWith(L".enc")) {
		fileName.MidFast(0, fileName.GetLength()-4); // 去除后缀
	}
	if(fileName.Find(L'.', 0)>0) {
		int i=0, cc=0;
		for (size_t j = idx+1; j < fileName.GetLength(); j++)
		{
			TCHAR ch = fileName.GetAt(j);
			if(isPunct(ch)) { // ch 是任意标点符号
				cc=0;
			}  
			else 
			{
				if((cc%2)!=0 || cc>5&&isPrime(cc)) {
					if(ch>=L'A'&& ch<=L'u') {
						auto idx = voals.Find(ch);
						if(idx>=0) ch = voals.GetAt((idx-1+voals.GetLength())%voals.GetLength());
						//DbgPrint(L"GetFilePath 解密 0  \n");
					}
					else if(ch>=(L'一')&& ch<=(L'龥')) {
						ch = ch-1;
						if(ch<L'一') ch=L'龥';
						//DbgPrint(L"GetFilePath 解密 1  \n");
					}
				}
				cc++;
			}
			//QkString tmp = ch;
			//DbgPrint(L"GetFilePath 解密 %s %d %d   \n",  tmp.GetData(), isPunct(ch), ch>=(L'一')&& ch<=(L'龥'));
			fileName.SetAt(j, ch);
			i++;
		}
		//QkString tmp = "x";
		//tmp.SetAt(0, L'嘠'-1);
		//DbgPrint(L"GetFilePath 解密文件名 = %s %d %d %s \n", fileName.GetData(), L'嘠'-L'一', L'龥'-L'嘠', tmp.GetData(), 0);
	}
}


set<QkString> touchedTags;
bool titleKey = false;

bool WODPlayer::PlayVideoFile(const TCHAR* path)
{
	bStopped = 0;
	bool ret = false;
	if (!_mMediaPlayer)
	{
		newVideoView();
	}
	//if(!isMain) return 1;
	//ASSERT(_mMediaPlayer);
	if (_currentPath!=path)
	{
		lastDuration = 0;
		_durationCache = 0;
		_currentPath = path;
		isPng = _currentPath.EndWith(L".webp") || _currentPath.EndWith(L".jpg") || _currentPath.EndWith(L".jpeg") || _currentPath.EndWith(L".png");
		fakePos = 0;
		// touch access time
		int st = _currentPath.ReverseFind(L"\\");
		int st1 = _currentPath.ReverseFind(L"/");
		st = MAX(st, st1)+1;
		int tagIdx = _currentPath.Find(L" - ", st);
		if(tagIdx==-1) tagIdx = _currentPath.GetLength();
		QkString tags = _currentPath.Mid(st, tagIdx-st);
		if(touchedTags.find(tags)==touchedTags.end() || 1) {
			//touchedTags.insert(tags);
			HANDLE hFile = CreateFile(_currentPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE) {
				FILETIME ft;
				GetSystemTimeAsFileTime(&ft);
				SetFileTime(hFile, NULL, &ft, NULL);
				CloseHandle(hFile);
			}
		}
		
	}
	nSkipStart = nSkipEnd = 0;
	int seekStart = _currentPath.Find(L"[p");
	if(seekStart>=0) {
		int seekEnd = _currentPath.Find(L"]", seekStart+2), sep=0;
		if(seekEnd>0) {
			sep = _currentPath.Find(L"-", seekStart);
			if(sep>=seekEnd)
				sep = -1;
			nSkipStart = parsint(_currentPath.GetData(), seekStart+2, sep>0?sep:seekEnd, 0);
			if(sep>0) {
				nSkipEnd = parsint(_currentPath.GetData(), sep+1, seekEnd, 0);
			}
		}
		//std::string buffer;
		//lxx(ss dd, _currentPath.Mid(seekStart+2, sep-seekStart-2).GetData(), (int)nSkipStart)
		//lxxx('seekEndseekEnd dd dd ss ', (int)nSkipStart, (int)nSkipEnd, _currentPath.Mid(seekStart, seekEnd-seekStart).GetData(buffer));
	}
	bool titKey = false;
	int endIdx = -1, idx;
	QkString title;
	title.Empty();
	bool save = false; 
	if(_currentPath.EndWith(L"NAMVOD")) {
		endIdx = _currentPath.Find(L"VODNAM");
		if(endIdx>0) {
			title = _currentPath.GetData()+endIdx+6;
			title.MidFast(0, title.GetLength()-6);
			save = true;
			titKey = true;
		}
	}
	//xpLogTo(_currentPath);
	if (_mMediaPlayer)
	{
		//_mMediaPlayer->setIni
		ret = _mMediaPlayer->PlayVideoFile(path, _currentPath.GetData(buffer, 0, endIdx));
	}

	if(!isMain) {
		hasTrack = true;
		return true;
	}

	titleKey = titKey;

	int ed, wenhao, b1=title.IsEmpty();
	if(b1) {
		idx = _currentPath.ReverseFind('/');
		ed = _currentPath.GetLength()-1;
		wenhao=-1;
		if(idx==-1) idx = _currentPath.ReverseFind('\\');
		if (_currentPath.StartWith(L"http"))
		{
			wenhao = _currentPath.ReverseFind('?');
			if(wenhao>idx+1) ed = wenhao-1;
		}
		title.Append(STR(_currentPath)+idx+1, ed-idx);
		if(title.EndWith(".enc")) {
			decryptFileName(title);
		}

	}

	_app->_titleBar->SetText(title);
	_app->_titleBar->SetNeedAutoCalcSize();


	if(b1) {
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
	}

	_app->_titleBar->NeedParentUpdate();
	::SetWindowText(_app->GetHWND(), STR(_app->_titleBar->GetText()));

	_timeLabel->GetText() = L"--:--:--";
	_durationLabel->GetText() = L"--:--:--";
	_durationLabel->NeedParentUpdate();

	_app->onNewVideo();

	if(!_seekbar._decorator)
	{
		_seekbar.SetTag((LONG_PTR)this);
		_seekbar._decorator = (SeekBarTrackDecorator)TimeMarksDecorator;
		_seekbar._callback = (SeekBarTrackCallback)seekchange;
		_seekfloat.SetTag((LONG_PTR)this);
		_seekfloat._decorator = (SeekBarTrackDecorator)TimeMarksFloatDecorator;
		_seekfloat._callback = (SeekBarTrackCallback)seekchangefloat;
	}
	// 获取书签
	//if (ret)
	{
		QkString tmp;

		_timeMarked = -1;

		QkString pathBuffer;
		pathBuffer.EnsureCapacity(_currentPath.GetLength() + 30);
		pathBuffer.AsBuffer();

		if (titKey) {
			_timeMarked = _app->_db->GetBookMarks("http"
				, _app->_titleBar->GetText().GetData(threadBuffer1), _bookmarks);
		}
		else if (DuiLib::PathCanonicalizeW((LPWSTR)pathBuffer.GetData(), _currentPath))
		{
			pathBuffer.RecalcSize();
			if(pathBuffer.Find('\"')) {
				pathBuffer.Replace(L"\"", L"\"\"");
			}
			auto idx = pathBuffer.Find('?');
			if(idx>0) {
				pathBuffer.MidFast(0, idx);
			}
			int fullPathLen = pathBuffer.GetLength();
			if (DuiLib::PathRemoveFileSpecW((LPWSTR)pathBuffer.GetData()))
			{
				pathBuffer.RecalcSize();
				size_t basePathLen = pathBuffer.GetLength();
				QkString fileName = pathBuffer.GetData()+basePathLen+1;//pathBuffer.GetData(threadBuffer, basePathLen+1, fullPathLen-basePathLen-1);
				if(fileName.EndWith(".enc")) {
					decryptFileName(fileName);
				}
				//lxx(,STR(fileName))
				//lxx(,STR(pathBuffer)+basePathLen+1)
				//LogIs(2, pathBuffer.GetData(threadBuffer, basePathLen+1, fullPathLen-basePathLen-1));
				_timeMarked = _app->_db->GetBookMarks(pathBuffer.GetData(threadBuffer1)
					, fileName.GetData(threadBuffer), _bookmarks);
			}
		}

		MarkPlaying();
		if(!_mMediaPlayer) {
			_mMediaPlayer = new DummyPlayer();
		}
		if (_mMediaPlayer->IsPaused())
		{
			_mMediaPlayer->Play();
		}
		return true;
	}
	return false;
}



long WODPlayer::GetDuration() 
{
	long duration = _mMediaPlayer->GetDuration();
	if(duration==0) {
		isPng = 1;
		duration = 2.3*1000; // 3 秒
	}
	lastDuration=MAX(lastDuration, duration);
	lastDuration=MAX(lastDuration, 10);
	return lastDuration;
}

extern bool _playing;


int WODPlayer::GetPosition(bool tick) 
{
	//lxxx(GetPosition dd dd, tick, _playing)
	int pos = _mMediaPlayer->GetPosition();
	if(isPng) {
		if(tick && _playing) fakePos += 200;
		pos = fakePos;
	}
	return pos;
}



void WODPlayer::SetPosition(long pos, bool fastSeek)
{
	if(_mMediaPlayer)
		_mMediaPlayer->SetPosition(pos, fastSeek);
	if(isPng) {
		fakePos = pos;
	}
}


int WODPlayer::AddBookmark()
{
	if(_mMediaPlayer) { // todo use binary insert
		int pos = _mMediaPlayer->GetPosition();
		int duration = GetDuration();
		//LogIs(2, L"%s", (LPCWSTR)_currentPath);


		QkString pathBuffer;
		pathBuffer.EnsureCapacity(_currentPath.GetLength() + 30);
		pathBuffer.AsBuffer();

		int rowId = -1;

		if (titleKey) {
			rowId = _app->_db->AddBookmark("http"
				, _app->_titleBar->GetText().GetData(threadBuffer1), 0, _timeMarked, pos, duration, 0);
		}
		else if (DuiLib::PathCanonicalizeW((LPWSTR)pathBuffer.GetData(), _currentPath))
		{
			pathBuffer.RecalcSize();
			if(pathBuffer.Find('\"')) {
				pathBuffer.Replace(L"\"", L"\"\"");
			}
			auto idx = pathBuffer.Find('?');
			if(idx>0) {
				pathBuffer.MidFast(0, idx);
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

				QkString fileName = pathBuffer.GetData()+basePathLen+1;//pathBuffer.GetData(threadBuffer, basePathLen+1, fullPathLen-basePathLen-1);
				if(fileName.EndWith(".enc")) {
					decryptFileName(fileName);
				}

				rowId = _app->_db->AddBookmark(pathBuffer.GetData(threadBuffer1)
					, fileName.GetData(threadBuffer), 0, _timeMarked, pos, duration, 0);
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
			idx = _bookmarks.size();
			_selectedBookmark = _bookmarks.size();
			_bookmarks.push_back({pos, rowId});
		}
		_seekbar.Invalidate();
		return idx;
	}
	return -1;
}

void WODPlayer::SelectBookMark(int index)
{
	if (index>=0 && index<_bookmarks.size())
	{
		SetPosition(_bookmarks[index].pos, false);
		_selectedBookmark = index;
		if (!_isPlaying)
		{
			_seekbar.SetProgress(_bookmarks[index].pos);
		}
	}
}

int WODPlayer::DelBookmark(int index)
{
	if (index>=0 && index<_bookmarks.size())
	{
		const auto & bkmk =  _bookmarks[index];
		_app->_db->DelBookmark(bkmk.rowID);
		_bakTime = bkmk.pos;
		_bookmarks.erase(_bookmarks.begin()+index);
		_seekbar.Invalidate();
		_selectedBookmark = -1;
		return index;
	}
	return -1;
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

			if (_scale > 1 && 0)
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

void WODPlayer::Toggle(int play)
{
	if (_mMediaPlayer)
	{
		bool toPlay;
		if(play==-1)
			toPlay = !_isPlaying;
		else 
			toPlay = play == 1;
		if (toPlay)
		{
			_mMediaPlayer->Play();
		}
		else 
		{
			_mMediaPlayer->Pause();
		}
		MarkPlaying(toPlay);
		if(!_hPlayer) // fixme
		lxx(%ld = %ld != %ld, _hPlayer, _mMediaPlayer->getHWND(), GetHWND())
	}
	if(isMain && hasTrack) {
		_app->_audioPlayer.Toggle(_isPlaying?1:0);
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
			if(!isMain) {
				return 1;
			}
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
