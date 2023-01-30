/** Copyright
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

#include <tchar.h>
#include <atlbase.h>
#include "PlayerSink.h"
#include "WindowBase.h"
#include "VideoPlayer.h"


enum PLAY_STATE
{
	PS_READY		= 0,
	PS_OPENING		= 1,
	PS_PAUSING		= 2,
	PS_PAUSED		= 3,
	PS_PLAYING		= 4,
	PS_PLAY			= 5,
	PS_CLOSING		= 6,
};

class CPlayerEventHandler
{
public:
	virtual	HRESULT	OnMessage(LONG nMessage, LONG wParam, LONG lParam) = 0;
	virtual	HRESULT	OnStateChanged(LONG nOldState, LONG nNewState) = 0;
	virtual	HRESULT	OnOpenSucceeded() = 0;
	virtual	HRESULT	OnSeekCompleted(LONG nPosition) = 0;
	virtual	HRESULT	OnBuffer(LONG nPercent) = 0;
	virtual	HRESULT	OnVideoSizeChanged(void) = 0;
	virtual	HRESULT	OnDownloadCodec(BSTR strCodecPath) = 0;
};

class VPlayerXunBo : public CPlayerEventHandler, public VideoPlayer
{
public:
	VPlayerXunBo(int & error_code, HINSTANCE hInstance, HWND hParent);
	~VPlayerXunBo();

public:
	int				GetConfigInt(int nConfigId);
	void			Initialize();
	BOOL			GetPathFromFullName(const TCHAR * pcszFullName, TCHAR * pszPath);
	BOOL			CreateAPlayerWindow();

	void			Stop() override;
	void			Play() override;
	void			Pause() override;
	bool			IsPlaying() override;
	bool			IsPaused() override;
	long			GetPosition() override;
	void			SetPosition(long pos) override;
	bool			PlayVideoFile(TCHAR* path) override;
	void			Close() override;
	void			Release() override;

	void			MillisecondToText(int nMs, TCHAR * pszText);
	HRESULT			CreateInstanceFromFile(const TCHAR * pcszPath, REFCLSID rclsid, 
										   REFIID riid, IUnknown * pUnkOuter, LPVOID * ppv);

	// aplayer event handler
	virtual	HRESULT	OnMessage(LONG nMessage, LONG wParam, LONG lParam);
	virtual	HRESULT	OnStateChanged(LONG nOldState, LONG nNewState);
	virtual	HRESULT	OnOpenSucceeded();
	virtual	HRESULT	GetDuration();
	virtual	HRESULT	OnSeekCompleted(LONG nPosition);
	virtual	HRESULT	OnBuffer(LONG nPercent);
	virtual	HRESULT	OnVideoSizeChanged(void);
	virtual	HRESULT	OnDownloadCodec(BSTR strCodecPath);

protected:
	LRESULT RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return ((VPlayerXunBo*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->RunProc(hwnd, message, wParam, lParam);
	}
	APlayer3Lib::IPlayer *	m_pAPlayer;
	//long						m_nPosition;
	//long						m_nDuration;

	IConnectionPoint *		m_pConnectionPoint;
	DWORD					m_dwCookie;
	HMODULE					m_hModule;
};
