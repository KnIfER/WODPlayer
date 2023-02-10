/** Copyright 2023 WODPLayer author
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 3
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
#include "ExternalPlayer.h"
#include "ExternalPlayerInterface.h"
#include "Resource.h"

#include <atlbase.h>
#include <atlhost.h>
#include "DuiLib\Core\InsituDebug.h"



ExternalPlayer::ExternalPlayer(int & error_code, HINSTANCE hInstance, HWND hParent, const TCHAR* dllPath)
{
	error_code=1;
	WindowBase::init(hInstance, hParent);
	_player=0;
	HMODULE hPlayer = vwInit(dllPath, true);
	error_code = vwCreatePlayer({hPlayer, hInstance, hParent, &_player});
	_hWnd = vwGetHWND(_player);
	LogIs("vwCreatePlayer=%d, _player=%ld", error_code, _player);
}

ExternalPlayer::~ExternalPlayer()
{
	Release();
	_player = 0;
}

void ExternalPlayer::Close()
{
	if (_player)
	{
		vwClose(_player);
	}
}

void ExternalPlayer::Release()
{
	if (_player)
	{
		vwRelease(_player);
	}
}

void ExternalPlayer::syncResolution()
{
	if (_player)
	{
		vwSynSize(_player, &_resX, &_resY);
	}
}

bool ExternalPlayer::PlayVideoFile(const TCHAR* path)
{
	LogIs(L"PlayVideoFile host :: path=%s, m_pAPlayer=%ld", path, _player);

	if (_player == NULL)
	{
		return false;
	}

	
	return vwPlayVideoFile(_player, path);
}

void ExternalPlayer::Play()
{
	if (_player != NULL)
	{
		vwPlay(_player);
	}
}

void ExternalPlayer::Pause()
{
	if (_player != NULL)
	{
		vwPause(_player);
	}
}

bool ExternalPlayer::IsPlaying()
{
	return _player!=NULL && vwIsPlaying(_player);
}

bool ExternalPlayer::IsPaused()
{
	return _player!=NULL && vwIsPaused(_player);
}

void ExternalPlayer::Stop()
{
	if (_player != NULL 
		&& (vwIsPlaying(_player)||vwIsPaused(_player))
		)
	{
		vwClose(_player);
	}
}

long ExternalPlayer::GetPosition()
{
	return _player?vwGetPosition(_player):0;
}

long ExternalPlayer::GetDuration()
{
	return _player?vwGetDuration(_player):0;
}

void ExternalPlayer::SetPosition(long pos)
{
	if (_player)
	{
		vwSetPosition(_player, pos);
	}
}


LRESULT ExternalPlayer::RunProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	switch (msg)
	{
	case WM_SETFOCUS:
		LogIs(2, "WM_SETFOCUS");
		return TRUE;
	default:
		break;
	}
	return ::CallWindowProc(_SysWndProc, hwnd, msg, w, l);
}