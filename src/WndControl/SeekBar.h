/** Copyright 2021 KnIfER JK. Chen
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
#pragma once


class SeekBar;

typedef void*(*SeekBarTrackCallback)(SeekBar*, int);

class SeekBar : public CContainerUI 
{
	DECLARE_QKCONTROL(SeekBar)
public:
	SeekBar();

	LPCTSTR GetClass() {
		return L"SeekBar";
	};

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

	void PaintStatusImage(HDC hDC);

	void DoEvent(TEventUI& event) override;

	void SetProgress(int progress, bool tracking=false);
	void SetMax(int progress);

	void SetMaxAndProgress(int progress, int max);


	bool GetIsSeeking();
	void SetThickness(int  val);
	void SetTrackColor(int  val);
	void SetProgressColor(int  val);
	void SetEnhancedTrackColor(int  val);
	void SetEnhancedProgressColor(int  val);
	void SetEnhanceThickness(int  val);
	void SetBarInset(int  val);
//protected:
	SeekBarTrackCallback _callback;
	bool _isSeeking=false;
	int _thickness = 3;
	int _trackColor;
	int _progressColor;
	int _enhancedTrackColor;
	int _enhancedProgressColor;
	int _enhanceThickness = 1;
	int _barInset = 5;

	int _max = 100000;
	int _progress = 50000;
private:
	int dragDownPos;
	int dragDownLeft;
	int dragDownWidth;
};



