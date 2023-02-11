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


VideoPlayer* initVidePlayerImpl(WODPlayer* xpp, int type)
{
	int error_code=-1;
	VideoPlayer* ret;
	type = 1;
	//type = 0;
	if (type==0)
	{
		ret = new ExternalPlayer(error_code, CPaintManagerUI::GetInstance(), xpp->GetHWND()
			, L"D:\\Code\\FigureOut\\XunLeiExternalPlayer\\bin\\XunLeiExternalPlayer.dll");
		if (SUCCEED)
		{
			return ret;
		}
		delete ret;
	}

	// vlc
	if (type==1)
	{
		ret = new ExternalPlayer(error_code, CPaintManagerUI::GetInstance(), xpp->GetHWND()
			, L"D:\\Code\\FigureOut\\Textrument\\plugins\\DirectUILib\\WODPlayer\\bin\\plugins\\VLCExternalPlayer.dll");
		if (SUCCEED)
		{
			return ret;
		}
		delete ret;
	}

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
}