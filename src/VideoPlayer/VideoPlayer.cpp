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

#include "WODWindow.h"
#include "SimplePlayer.h"
#include "VLCPlayer.h"
//#include "MFPlayer2.h"

#define SUCCEED error_code==0

VideoPlayer* initVidePlayerImpl(WODWindow* xpp, int type)
{
	int error_code=-1;
	VideoPlayer* ret;
	//type = 1;
	if (type==0)
	{
		ret = new VPlayerXunBo(error_code, xpp->getInstace(), xpp->getHWND());
		if (SUCCEED)
		{
			return ret;
		}
		delete ret;
	}
	/*vlc 播放器不支持硬件加速
	 	   启用此需要 vlc sdk，并设置相关路径于 VLCPaths.props，记得将相关源码(VLCPlayer.cpp)拖入项目。*/
	if (type==1)
	{
		ret = new VLCPlayer(error_code, xpp->getInstace(), xpp->getHWND());
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