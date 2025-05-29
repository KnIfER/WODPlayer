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

#include "pch.h"

#include "ExternalPlayer.h"

//#include "VLCPlayer.h"
//#include "MFPlayer2.h"


#define SUCCEED error_code==0


VideoPlayer* initVidePlayerImpl(WODPlayer* xpp, const TCHAR* pluginName, bool isMain)
{
	int error_code=-1;
	VideoPlayer* ret;
	if (0)
	{
		ret = new ExternalPlayer(error_code, CPaintManagerUI::GetInstance(), xpp->GetHWND()
			//, L"D:/Code/FigureOut/XunLeiExternalPlayer/bin/XunLeiExternalPlayer.dll");
			, L"D:/Code/FigureOut/Textrument/plugins/DirectUILib/WODPlayer/bin/plugins/MPVExternalPlayer.dll");
		if (SUCCEED)
		{
			return ret;
		}
		delete ret;
	}
	// vlc
	TCHAR buffer[MAX_PATH];
	::GetModuleFileName(CPaintManagerUI::GetInstance(), buffer, MAX_PATH);
	::PathRemoveFileSpec(buffer);
	::PathAppend(buffer, L"plugins");
	::PathAppend(buffer, pluginName);
	if(::PathFileExists(buffer)) 
	{
		QkString name = pluginName;
		LPCWSTR dllDir = 0;
		QkString testPath;
		auto dir1 = GetProfString(name.GetData(threadBuffer));
		if(dir1 && ::PathFileExistsA(dir1->c_str())) 
		{
			testPath = dir1->c_str();
			dllDir = testPath;
		}
		if (!dllDir && name.EndWith(L"VLCExternalPlayer.dll")) // auto detect
		{
			testPath = buffer;
			testPath.AsBuffer();
			::PathRemoveFileSpec((LPWSTR)STR(testPath));
			::PathAppend((LPWSTR)STR(testPath), L"libvlc.dll");
			if(!::PathFileExists(STR(testPath))) 
			{
				testPath.Empty();
				testPath = L"C:/";
				testPath += L"Program Files";
#ifndef _WIN64
				testPath += L" (x86)";
#endif
				testPath += L"/VideoLAN/";
				if(::PathFileExists(STR(testPath))) 
				{
					LPWSTR installed = (LPWSTR)STR(testPath);
					//testPath += L"vlc-2.2.8";
					//testPath += L"vlc-3.0.1x";
					testPath += L"vlc-3.0.18";
					if(::PathFileExists(STR(testPath))) 
					{
						dllDir = testPath;
					}
					else
					{
						testPath.SetAt(testPath.GetLength()-10, 0);
						testPath.RecalcSize();
						testPath += L"VLC";
						if(::PathFileExists(STR(testPath))) 
						{
							dllDir = testPath;
						}
					}
				}

			}
		}
		//LogIs(2, dllDir?dllDir:L"null");
		ret = new ExternalPlayer(error_code, CPaintManagerUI::GetInstance(), isMain?xpp->GetHWND():NULL, buffer, dllDir);
		if (SUCCEED)
		{
			return ret;
		}
		delete ret;
	}
	else error_code = 2;

	//::MessageBox(NULL, L"", TEXT("WODPlayer"), MB_OK);
	LogIs(2, "播放器加载失败！\n请尝试切换播放插件。\n错误码：%d", error_code);

	// MF 框架支持的格式太少，弃。
	//if (type==2)
	//{
	//	ret = new MFPlayer2(error_code, xpp->getInstace(), xpp->getHWND());
	//	if (SUCCEED)
	//	{
	//		return ret;
	//	}
	//	delete ret;
	//}
	return 0;
}



bool copyimage(const wchar_t* filename)
{
	//initialize Gdiplus once:
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	bool result = false;
	Gdiplus::Bitmap* gdibmp = Gdiplus::Bitmap::FromFile(filename);
	//LogIs(2, "succ= %d  ", gdibmp);
	if (gdibmp)
	{
		HBITMAP hbitmap;
		gdibmp->GetHBITMAP(0, &hbitmap);
		if (OpenClipboard(NULL))
		{
			EmptyClipboard();
			DIBSECTION ds;
			if (GetObject(hbitmap, sizeof(DIBSECTION), &ds))
			{
				HDC hdc = GetDC(HWND_DESKTOP);
				//create compatible bitmap (get DDB from DIB)
				HBITMAP hbitmap_ddb = CreateDIBitmap(hdc, &ds.dsBmih, CBM_INIT,
					ds.dsBm.bmBits, (BITMAPINFO*)&ds.dsBmih, DIB_RGB_COLORS);
				ReleaseDC(HWND_DESKTOP, hdc);
				SetClipboardData(CF_BITMAP, hbitmap_ddb);
				DeleteObject(hbitmap_ddb);
				result = true;
			}
			CloseClipboard();
		}

		//cleanup:
		DeleteObject(hbitmap);
		delete gdibmp;
	}
	return result;
}

bool copyimage(HBITMAP hbitmap)
{
	if (hbitmap && OpenClipboard(NULL)) {
		EmptyClipboard();
		SetClipboardData(CF_BITMAP, hbitmap);
		CloseClipboard();
		return 1;
	}
	return 0;
}

bool copyimage_1(HBITMAP hbitmap)
{
	if (hbitmap)
	{
		DIBSECTION ds;
		if (GetObject(hbitmap, sizeof(DIBSECTION), &ds))
		{
			HDC hdc = GetDC(HWND_DESKTOP);
			//create compatible bitmap (get DDB from DIB)
			HBITMAP hbitmap_ddb = CreateDIBitmap(hdc, &ds.dsBmih, CBM_INIT,
				ds.dsBm.bmBits, (BITMAPINFO*)&ds.dsBmih, DIB_RGB_COLORS);
			ReleaseDC(HWND_DESKTOP, hdc);
			copyimage(hbitmap_ddb);
			DeleteObject(hbitmap_ddb);
		}
		return 1;
	}
	return 0;
}
