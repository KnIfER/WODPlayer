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
	RECT rcBar = rc;
	if(_barInset) {
		rcBar.left += _barInset;
		rcBar.right -= _barInset;
	}

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


	int radius = 6;
	int zjiin = 11;

	RECT tmpRc{rcBar.right-radius, GetHeight()/2-radius, 0, 0};

	trackBrush.SetColor(0xffeeeeee);


	graphi.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);// SmoothingMode

	graphi.FillPie(&trackBrush, (INT)tmpRc.left, tmpRc.top, zjiin, zjiin, 0, 360);

	//graphi.FillEllipse(&trackBrush, (INT)tmpRc.left, tmpRc.top
	//	, zjiin
	//	, zjiin);



	//if( (_trackColor & 0xFF000000)==0 ) return;
	//Gdiplus::Graphics graphi( hDC );
	//Gdiplus::SolidBrush brush(Gdiplus::Color((LOBYTE((color)>>24)), GetBValue(color), GetGValue(color), GetRValue(color)));
	//graphi.FillRectangle(&brush, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);

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

		SetProgress(_max * (dragDownPos-dragDownLeft)*1.0/dragDownWidth, true);
	}
	if(_isSeeking) {
		if(event.Type==UIEVENT_BUTTONUP) {
			//LogIs(2, "UIEVENT_BUTTONDOWN!!!");
			_isSeeking = false;
		}
		if(event.Type==UIEVENT_MOUSEMOVE) {
			SetProgress(_max * (event.ptMouse.x-dragDownLeft)*1.0/dragDownWidth, true);
		}
	}
}

void SeekBar::SetProgress(int progress, bool tracking)
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

void SeekBar::SetMax(int max)
{
	if (_max != max)
	{
		_max = max;
		if(_progress>_max) _progress=_max;
		Invalidate();
	}
}

void SeekBar::SetMaxAndProgress(int progress, int max)
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
