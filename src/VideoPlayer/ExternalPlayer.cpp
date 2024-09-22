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
#include "Resource.h"

#include <atlbase.h>
#include <atlhost.h>
#include "DuiLib\Core\InsituDebug.h"


// load module
static void PRINTMSG(TCHAR* buff, const CHAR* name, int & printed_len)
{
	int i=0, len=(int)strlen(name);
	if(printed_len+len+6<74)
	{
		for(;i<len;i++) 
		{
			buff[printed_len+i]=name[i];
		}
		buff[printed_len+i]=' ';
		printed_len+=len+1;
	}
	else if(printed_len<74)
	{
		buff[++printed_len]='.';
	}
}

#define DEF_FUNC(hMod, func, type, name)\
	func = (type)GetProcAddress(hMod, name);\
	if(!func) PRINTMSG(PRINTBUFF, name, PRINTLEN);

#define DEF_FUNC_NOWARN(hMod, func, type, name)\
	func = (type)GetProcAddress(hMod, name);

static HMODULE _hPlayerMod;

HMODULE ExternalPlayer::vwInit(int & error_code, const TCHAR* dllPath, bool blame, const TCHAR* dllDir)
{
	if(PathFileExists(dllPath))
	{
		const DWORD dwFlags = GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "AddDllDirectory") != NULL ? LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS : 0;
		::SetDllDirectory(dllDir);
		//_unloaddll((intptr_t)_hPlayerMod);
		//auto x=::LoadLibraryEx(L"D:/Code/FigureOut/Textrument/plugins/DirectUILib/WODPlayer/bin/plugins/libmpv-2.dll", NULL, dwFlags);
		//auto x=::LoadLibrary(L"D:/Code/FigureOut/Textrument/plugins/DirectUILib/WODPlayer/bin/plugins/libmpv-2.dll");
		auto hPlayer = ::LoadLibraryEx(dllPath, NULL, dwFlags);
		//_hPlayerMod = hPlayer;
		if(hPlayer) 
		{
			TCHAR PRINTBUFF[75]=L"player api not found : ";
			int PRINTLEN=lstrlen(PRINTBUFF);
			int PRINTLEN_0=PRINTLEN;
			DEF_FUNC(hPlayer, vwCreatePlayer, VW_CREATEPLAYER, "vwCreatePlayer");
			DEF_FUNC(hPlayer, vwGetHWND, VW_GETHWND, "vwGetHWND");
			DEF_FUNC(hPlayer, vwRelease, VW_RELEASE, "vwRelease");
			DEF_FUNC(hPlayer, vwPlay, VW_PLAY, "vwPlay");
			DEF_FUNC(hPlayer, vwPause, VW_PAUSE, "vwPause");
			DEF_FUNC(hPlayer, vwStop, VW_STOP, "vwStop");
			DEF_FUNC(hPlayer, vwIsPlaying, VW_ISPLAYING, "vwIsPlaying");
			DEF_FUNC(hPlayer, vwIsPaused, VW_ISPAUSED, "vwIsPaused");
			DEF_FUNC(hPlayer, vwGetPosition, VW_GETPOSITION, "vwGetPosition");
			DEF_FUNC(hPlayer, vwGetDuration, VW_GETDURATION, "vwGetDuration");
			DEF_FUNC(hPlayer, vwSetPosition, VW_SETPOSITION, "vwSetPosition");
			DEF_FUNC(hPlayer, vwSetLoop, VW_SETLOOP, "vwSetLoop");
			DEF_FUNC_NOWARN(hPlayer, vwSetInitialPosition, VW_SETINITIALPOSITION, "vwSetInitialPosition");
			DEF_FUNC(hPlayer, vwSetFullScreen, VW_SETFULLSCREEN, "vwSetFullScreen");
			DEF_FUNC(hPlayer, vwPlayVideoFile, VW_PLAYVIDEOFILE, "vwPlayVideoFile");
			DEF_FUNC(hPlayer, vwSyncSize, VW_SYNCSIZE, "vwSyncSize");
			DEF_FUNC(hPlayer, vwClose, VW_CLOSE, "vwClose");
			DEF_FUNC(hPlayer, vwInterface, VW_INTERFACE, "vwInterface");
			DEF_FUNC(hPlayer, vwSetRate, VW_SETRATE, "vwSetRate");
			DEF_FUNC(hPlayer, vwSetVolume, VW_SETVOLUME, "vwSetVolume");
			DEF_FUNC(hPlayer, vwSetRotation, VW_SETROTATION, "vwSetRotation");
			DEF_FUNC(hPlayer, vwGetRotation, VW_GETROTATION, "vwGetRotation");
			DEF_FUNC(hPlayer, vwSetPositionEx, VW_SETPOSITIONEX, "vwSetPositionEx");
			if(PRINTLEN!=PRINTLEN_0)
			{
				PRINTBUFF[PRINTLEN]='\0';
				LogIs(1, PRINTBUFF);
				if(blame)
				{
					::MessageBox(NULL, PRINTBUFF, TEXT("External Video Player Widget"), MB_OK);
				}
			}
			//LogIs(2, "%ld %ld loaded", hPlayer, vwCreatePlayer);
			return vwCreatePlayer!=0?hPlayer:0;
		}
		else error_code = 13;
	}
	else error_code = 12;
	return 0;
}

#include <stdexcept>

ExternalPlayer::ExternalPlayer(int & error_code, HINSTANCE hInstance, HWND hParent, const TCHAR* dllPath, const TCHAR* dllDir)
{
	error_code=1;
	WindowBase::init(hInstance, hParent);
	_player=0;
	try
	{
		HMODULE hPlayer = vwInit(error_code, dllPath, true, dllDir);
		if(!hPlayer) 
		{
			return;
		}
		error_code = vwCreatePlayer({hPlayer, hInstance, hParent, &_player, dllDir});
		if(error_code==0)
			_hWnd = vwGetHWND(_player);
	} 
	catch (int i)
	{
		TCHAR str[50] = TEXT("God Damned Exception : ");
		TCHAR code[10];
		wsprintf(code, TEXT("%d"), i);
		wcscat_s(str, code);
		LogIs(str);
		error_code = 6;
	}
	catch (std::runtime_error & ex)
	{
		LogIs("General Runtime Exception %s", ex.what());
		error_code = 5;
	}
	//catch (const Win32Exception & ex)
	//{
	//	TCHAR message[1024];	//TODO: sane number
	//	wsprintf(message, TEXT("An exception occured. Notepad++ cannot recover and must be shut down.\r\nThe exception details are as follows:\r\n")
	//		TEXT("Code:\t0x%08X\r\nType:\t%S\r\nException address: 0x%p"), ex.code(), ex.what(), ex.where());
	//	::MessageBox(Notepad_plus_Window::gNppHWND, message, TEXT("Win32Exception"), MB_OK | MB_ICONERROR);
	//	mdump.writeDump(ex.info());
	//	doException(notepad_plus_plus);
	//}
	catch (std::exception & ex)
	{
		LogIs("General Exception %s", ex.what());
		error_code = 4;
	}
	catch(...) 
	{
		error_code = 3;
	}
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

void ExternalPlayer::syncResolution(unsigned int & _resX, unsigned int & _resY)
{
	if (_player && vwSyncSize)
	{
		vwSyncSize(_player, &_resX, &_resY);
	}
}

float ExternalPlayer::SetRate(float rate)
{
	if(_player && vwSetRate) 
	{
		return vwSetRate(_player, rate);
	}
	return rate;
}

int ExternalPlayer::SetVolume(int volume)
{
	if(_player && vwSetVolume) 
	{
		return vwSetVolume(_player, volume);
	}
	return volume;
}



void ExternalPlayer::SetRotation(int value)
{
	if(_player && vwSetRotation) 
	{
		return vwSetRotation(_player, value);
	}
}

int ExternalPlayer::GetRotation()
{
	if(_player && vwSetVolume) 
	{
		return vwGetRotation(_player);
	}
	return 0;
}



int ExternalPlayer::SetPositionEx(LONG wParam, LONG LPARAM)
{
	if(_player && vwSetPositionEx) 
	{
		return vwSetPositionEx(_player, wParam, LPARAM);
	}
	return 0;
}


bool ExternalPlayer::PlayVideoFile(const TCHAR* path, const CHAR* path1)
{
	LogIs(L"PlayVideoFile host :: path=%s, m_pAPlayer=%ld", path, _player);

	if (_player == NULL)
	{
		return false;
	}

	
	return vwPlayVideoFile(_player, path, path1);
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
	//return 1000*60*60;
}

void ExternalPlayer::SetPosition(long pos, bool fastSeek)
{
	if (_player)
	{
		vwSetPosition(_player, pos, fastSeek);
	}
}

void ExternalPlayer::SetLoop(bool loop)
{
	if (_player)
	{
		vwSetLoop(_player, loop);
	}
}

void ExternalPlayer::SetInitialPosition(long pos)
{
	if (_player)
	{
		vwSetInitialPosition(_player, pos);
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
