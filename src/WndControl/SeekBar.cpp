/** Copyright 2023 WinQkUI author
* 
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU Lesser General Public License as published by the
* Free Software Foundation; either version 2.1 of the License, or (at
* your option) any later version. 
* 
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* A copy of the GNU Lesser General Public License is included in the
* distribution in the file COPYING.LIB. If you did not receive this
* copy, write to the Free Software Foundation.
*/
#include "pch.h"


IMPLEMENT_QKCONTROL(SeekBar)

SeekBar::SeekBar(){
	_trackColor = 0x70888888;
	_progressColor = 0xff958700;
	_enhancedTrackColor    = 0x20888888;
	_enhancedProgressColor = 0x20565d06;

	_thumbSize = 12;
	_thumbColor = 0xffeeeeee;
	_thumbColorTracking = 0x80eeeeee;

	_enhanceThickness = 1;
	_barInset = 5;
	_thickness = 3;

	_max = 100000;
	_progress = 50000;
}

void SeekBar::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	//if( _tcsicmp(pstrName, _T("mousechild")) == 0 ) setThickness(_tcsicmp(pstrValue, _T("true")) == 0);
	if( _tcsicmp(pstrName, _T("barinset")) == 0 ) SetBarInset(ParseInt(pstrValue));
	// m n o p q r s t
	else if(*pstrName>='m' && *pstrName<='t') {
		if( _tcsicmp(pstrName, _T("thickness")) == 0 ) SetThickness(ParseInt(pstrValue));
		else if( _tcsicmp(pstrName, _T("trackcolor")) == 0 ) SetTrackColor(ParseInt(pstrValue));
		else if( _tcsicmp(pstrName, _T("progress")) == 0 ) SetProgress(ParseInt(pstrValue));
		else if( _tcsicmp(pstrName, _T("max")) == 0 ) SetMax(ParseInt(pstrValue));
		else if( _tcsicmp(pstrName, _T("progresscolor")) == 0 ) SetProgressColor(ParseInt(pstrValue));
		else if( _tcsicmp(pstrName, _T("progresscolor1")) == 0 ) SetEnhancedProgressColor(ParseInt(pstrValue));
		else if( _tcsicmp(pstrName, _T("trackcolor1")) == 0 ) SetEnhancedTrackColor(ParseInt(pstrValue));
		else if( _tcsicmp(pstrName, _T("thickness1")) == 0 ) SetEnhanceThickness(ParseInt(pstrValue));
		else if( _tcsicmp(pstrName, _T("thumbsize")) == 0 ) SetEnhanceThickness(ParseInt(pstrValue));
		else if( _tcsicmp(pstrName, _T("thumbcolor")) == 0 ) SetThumbColor(ParseInt(pstrValue));
		else if( _tcsicmp(pstrName, _T("thumbcolor1")) == 0 ) SetThumbColorTraking(ParseInt(pstrValue));
		else __super::SetAttribute(pstrName, pstrValue);
	}
	else __super::SetAttribute(pstrName, pstrValue);
}


bool SeekBar::GetIsSeeking()
{
	return _isSeeking;
}

void SeekBar::SetThickness(int val)
{
	if(_thickness!=val) {
		_thickness = val;
		Invalidate();
	}
}

void SeekBar::SetTrackColor(int val)
{
	if(_trackColor!=val) {
		_trackColor = val;
		Invalidate();
	}
}

void SeekBar::SetProgressColor(int val)
{
	if(_progressColor!=val) {
		_progressColor = val;
		Invalidate();
	}
}

void SeekBar::SetEnhancedTrackColor(int val)
{
	if(_enhancedTrackColor!=val) {
		_enhancedTrackColor = val;
		Invalidate();
	}
}

void SeekBar::SetEnhancedProgressColor(int val)
{
	if(_enhancedProgressColor!=val) {
		_enhancedProgressColor = val;
		Invalidate();
	}
}

void SeekBar::SetThumbSize(int val)
{
	if(_thumbSize!=val) {
		_thumbSize = val;
		Invalidate();
	}
}

void SeekBar::SetThumbColor(int val)
{
	if(_thumbColor!=val) {
		_thumbColor = val;
		Invalidate();
	}
}

void SeekBar::SetThumbColorTraking(int val)
{
	if(_thumbColorTracking!=val) {
		_thumbColorTracking = val;
		Invalidate();
	}
}

void SeekBar::SetEnhanceThickness(int val)
{
	if(_enhanceThickness!=val) {
		_enhanceThickness = val;
		Invalidate();
	}
}

void SeekBar::SetBarInset(int val)
{
	if(_barInset!=val) {
		_barInset = val;
		Invalidate();
	}
}

void SeekBar::PaintStatusImage(HDC hDC)
{
	__super::PaintStatusImage(hDC);

	RECT rc = m_rcItem;
	ApplyInsetToRect(rc);
	if(_barInset) {
		rc.left += _barInset;
		rc.right -= _barInset;
	}
	RECT rcBar = rc;
	//int rcLeft = rcBar.left, rcRight = rcBar.right;
	rcBar.top = (GetHeight() - _thickness) / 2;
	rcBar.bottom = rcBar.top + _thickness;

	CRenderEngine::DrawColor(hDC, rcBar, _trackColor);


	Gdiplus::Graphics graphi( hDC );
	DWORD color = _trackColor;
	Gdiplus::SolidBrush trackBrush(_trackColor);
	//color = _progressColor;
	//Gdiplus::SolidBrush progressBrush(Gdiplus::Color((LOBYTE((color)>>24)), GetBValue(color), GetGValue(color), GetRValue(color)));
	
	graphi.FillRectangle(&trackBrush, (INT)rcBar.left, rcBar.top
		, rcBar.right - rcBar.left
		, rcBar.bottom - rcBar.top);
	
	if(_enhanceThickness) {
		trackBrush.SetColor(_enhancedTrackColor);
		graphi.FillRectangle(&trackBrush, rcBar.left, rcBar.top, rcBar.right - rcBar.left, _enhanceThickness);
		graphi.FillRectangle(&trackBrush, rcBar.left, rcBar.bottom - _enhanceThickness, rcBar.right - rcBar.left, _enhanceThickness);
	}

	rcBar.right = rcBar.left + (rcBar.right-rcBar.left)*1.0*_progress/_max;

	//CRenderEngine::DrawColor(hDC, rcBar, _progressColor);

	trackBrush.SetColor(_progressColor);

	graphi.FillRectangle(&trackBrush, (INT)rcBar.left, rcBar.top
		, rcBar.right - rcBar.left
		, rcBar.bottom - rcBar.top);

	if(_enhanceThickness) {
		trackBrush.SetColor(_enhancedProgressColor);
		graphi.FillRectangle(&trackBrush, rcBar.left, rcBar.top, rcBar.right - rcBar.left, _enhanceThickness);
		graphi.FillRectangle(&trackBrush, rcBar.left, rcBar.bottom - _enhanceThickness, rcBar.right - rcBar.left, _enhanceThickness);
	}


	int radius = _thumbSize/2;

	RECT tmpRc{rcBar.right-radius, GetHeight()/2-radius, 0, 0};


	if(_decorator) _decorator(this, graphi, trackBrush, rc);

	trackBrush.SetColor(_isSeeking?_thumbColorTracking:_thumbColor);

	graphi.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);// SmoothingMode

	graphi.FillPie(&trackBrush, (INT)tmpRc.left, tmpRc.top, _thumbSize, _thumbSize, 0, 360);
}

void SeekBar::DoEvent(TEventUI& event)
{
	if(event.Type==UIEVENT_BUTTONDOWN) {
		//LogIs(2, "UIEVENT_BUTTONDOWN!!!");
		_isSeeking = true;
		dragDownPos = event.ptMouse.x;
		RECT rc = m_rcItem;
		ApplyInsetToRect(rc);
		if(_barInset) {
			rc.left += _barInset;
			rc.right -= _barInset;
		}

		dragDownLeft = rc.left;
		dragDownWidth = rc.right - dragDownLeft;

		SetProgress((dragDownPos-dragDownLeft)*1.0/dragDownWidth * _max, true);
	}
	if(_isSeeking) {
		if(event.Type==UIEVENT_BUTTONUP) {
			//LogIs(2, "UIEVENT_BUTTONDOWN!!!");
			_isSeeking = false;
			Invalidate();
		}
		if(event.Type==UIEVENT_MOUSEMOVE) {
			SetProgress((event.ptMouse.x-dragDownLeft)*1.0/dragDownWidth * _max, true);
		}
	}
}

void SeekBar::SetProgress(LONG progress, bool tracking)
{
	if(progress < 0) progress = 0;
	else if(progress > _max) progress = _max;
	if (_progress != progress)
	{
		_progress = progress;
		if(tracking && _callback) {
			(*_callback)(this, progress);
		}
		Invalidate();
	}
}

void SeekBar::SetMax(LONG max)
{
	if (_max != max)
	{
		_max = max;
		if(_progress>_max) _progress=_max;
		Invalidate();
	}
}

void SeekBar::SetProgressAndMax(LONG progress, LONG max)
{
	if (_progress != progress || max!=_max)
	{
		_max = max;
		if(progress < 0) progress = 0;
		else if(progress > max) progress = max;
		_progress = progress;
		Invalidate();
	}
}
