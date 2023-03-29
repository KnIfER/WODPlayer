/** Copyright 2021 WODPLayer author
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
#include <Windows.h>
#include <Windowsx.h>
#include "VideoPlayerInerface.h"

#define SKIP_SKIA_RENDERER
#include "DuiLib\UIlib.h"
#include <Duilib/Core/InsituDebug.h>
#include "Utils/ProfileStd.h"

//#include "../WndControl/Seekbar_old.h"
#include "../WndControl/SeekBar.h"

#include "WODPlayer.h"
#include "WODApplication.h"


extern LONG_PTR WOD_IMG_UTILS(const char* msg, HWND hWnd=0, LONG_PTR wParam=0, LONG_PTR lParam=0);

#define SendWndMessage(Msg, wParam, lParam) SendMessage(_hWnd, Msg, wParam, lParam)

__declspec(selectany)  std::string threadBuffer;

__declspec(selectany)  std::string threadBuffer1;

__declspec(selectany)  DWORD TransparentKey;


extern bool IsKeyDown(int key);




