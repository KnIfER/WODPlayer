/** Copyright 2023 WODPLayer author
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
#include <windows.h>
#include <shlwapi.h>
#include <string>

struct VWCreateOptions{
	HINSTANCE hPlugin=0;
	HINSTANCE hHost=0;
	HWND hParent=0;
	LONG_PTR* ret;
};

typedef int (__cdecl* VW_CREATEPLAYER)(VWCreateOptions);
typedef HWND (__cdecl* VW_GETHWND)(LONG_PTR);
typedef void (__cdecl* VW_RELEASE)(LONG_PTR);
typedef void (__cdecl* VW_PLAY)(LONG_PTR);
typedef void (__cdecl* VW_PAUSE)(LONG_PTR);
typedef void (__cdecl* VW_STOP)(LONG_PTR);
typedef bool (__cdecl* VW_ISPLAYING)(LONG_PTR);
typedef bool (__cdecl* VW_ISPAUSED)(LONG_PTR);
typedef long (__cdecl* VW_GETPOSITION)(LONG_PTR);
typedef long (__cdecl* VW_GETDURATION)(LONG_PTR);
typedef void (__cdecl* VW_SETPOSITION)(LONG_PTR, long);
typedef void (__cdecl* VW_SETFULLSCREEN)(LONG_PTR, bool);
typedef bool (__cdecl* VW_PLAYVIDEOFILE)(LONG_PTR, const TCHAR*);
typedef bool (__cdecl* VW_CLOSE)(LONG_PTR);
typedef bool (__cdecl* VW_SYNCSIZE)(LONG_PTR, unsigned int*, unsigned int*);

// D:\Code\FigureOut\XunLeiExternalPlayer\bin\XunLeiExternalPlayer.dll

__declspec(selectany) /*static*/ VW_CREATEPLAYER vwCreatePlayer = nullptr;
__declspec(selectany) /*static*/ VW_GETHWND vwGetHWND = nullptr;
__declspec(selectany) /*static*/ VW_RELEASE vwRelease = nullptr;
__declspec(selectany) /*static*/ VW_PLAY vwPlay = nullptr;
__declspec(selectany) /*static*/ VW_PAUSE vwPause = nullptr;
__declspec(selectany) /*static*/ VW_STOP vwStop = nullptr;
__declspec(selectany) /*static*/ VW_ISPLAYING vwIsPlaying = nullptr;
__declspec(selectany) /*static*/ VW_ISPAUSED vwIsPaused = nullptr;
__declspec(selectany) /*static*/ VW_GETPOSITION vwGetPosition = nullptr;
__declspec(selectany) /*static*/ VW_GETDURATION vwGetDuration = nullptr;
__declspec(selectany) /*static*/ VW_SETPOSITION vwSetPosition = nullptr;
__declspec(selectany) /*static*/ VW_SETFULLSCREEN vwSetFullScreen = nullptr;
__declspec(selectany) /*static*/ VW_PLAYVIDEOFILE vwPlayVideoFile = nullptr;
__declspec(selectany) /*static*/ VW_CLOSE vwClose = nullptr;
__declspec(selectany) /*static*/ VW_SYNCSIZE vwSynSize = nullptr;

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

static HMODULE vwInit(const TCHAR* dllPath, bool blame=false)
{
	if(PathFileExists(dllPath))
	{
		const DWORD dwFlags = GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "AddDllDirectory") != NULL ? LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS : 0;
		auto hPlayer = ::LoadLibraryEx(dllPath, NULL, dwFlags);
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
			DEF_FUNC(hPlayer, vwSetFullScreen, VW_SETFULLSCREEN, "vwSetFullScreen");
			DEF_FUNC(hPlayer, vwPlayVideoFile, VW_PLAYVIDEOFILE, "vwPlayVideoFile");
			DEF_FUNC(hPlayer, vwSynSize, VW_SYNCSIZE, "vwSynSize");
			DEF_FUNC(hPlayer, vwClose, VW_CLOSE, "vwClose");
			if(blame && PRINTLEN!=PRINTLEN_0)
			{
				PRINTBUFF[PRINTLEN]='\0';
				::MessageBox(NULL, PRINTBUFF, TEXT("External Video Player Widget"), MB_OK);
			}
			return vwCreatePlayer!=0?hPlayer:0;
		}
	}
	return 0;
}
