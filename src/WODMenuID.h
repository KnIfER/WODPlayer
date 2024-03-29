#pragma once

#define IDM 40000

// 文件
#define IDM_FILE IDM
#define IDM_FILE_OPEN            IDM_FILE+1
#define IDM_FILE_CLOSE           IDM_FILE+2
#define IDM_FILE_RECENT          IDM_FILE+3
#define IDM_SHUTDOWN             IDM_FILE+4
#define IDM_DELETE               IDM_FILE+5
#define IDM_DELETE_FOREVER       IDM_FILE+6


// 播放
#define IDM_PLAY IDM + 1000
#define IDM_FULLSCREEN        IDM_PLAY+1
#define IDM_MAXMISE           IDM_PLAY+2
#define IDM_PAUSE             IDM_PLAY+3

#define IDM_SEEK_FORE         IDM_PLAY+4
#define IDM_SEEK_FORE_FAST    IDM_PLAY+5
#define IDM_SEEK_FORE_FASTER  IDM_PLAY+6
#define IDM_SEEK_BACK         IDM_PLAY+7
#define IDM_SEEK_BACK_FAST    IDM_PLAY+8
#define IDM_SEEK_BACK_FASTER  IDM_PLAY+9

#define IDM_SPEED_UP         IDM_PLAY+10
#define IDM_SPEED_DOWN	     IDM_PLAY+11
#define IDM_SPEED_UP_FAST        IDM_PLAY+12
#define IDM_SPEED_DOWN_FAST	     IDM_PLAY+13
#define IDM_SPEED_UP_FASTER      IDM_PLAY+14
#define IDM_SPEED_DOWN_FASTER	 IDM_PLAY+15
#define IDM_SPEED_RESET			 IDM_PLAY+16

#define IDM_VOLUME_UP         IDM_PLAY+17
#define IDM_VOLUME_DOWN		  IDM_PLAY+18

#define IDM_PLAY_PRV          IDM_PLAY+19
#define IDM_PLAY_NXT		  IDM_PLAY+20


// 书签
#define IDM_BKMK IDM + 2000
#define IDM_BKMK_PRV IDM_BKMK+1
#define IDM_BKMK_NXT IDM_BKMK+2
#define IDM_BKMK_DEL IDM_BKMK+3
#define IDM_BKMK_ADD IDM_BKMK+4
#define IDM_BKMK_RETURN IDM_BKMK+5


// 插件
#define IDM_PLUGIN IDM + 3000
#define IDM_PLUGIN_MF  IDM_PLUGIN+1
#define IDM_PLUGIN_VLC IDM_PLUGIN+2
#define IDM_PLUGIN_XL  IDM_PLUGIN+3



// 皮肤
#define IDM_SKIN IDM + 5000
#define IDM_SKIN_NORM         IDM_SKIN+1 
#define IDM_SKIN_HOLLOW       IDM_SKIN+2 
#define IDM_SKIN_ALPHA_1      IDM_SKIN+3 
#define IDM_SKIN_ALPHA_2      IDM_SKIN+4 
#define IDM_SKIN_ALPHA_3      IDM_SKIN+5 
#define IDM_SKIN_ALPHA_4      IDM_SKIN+6 
#define IDM_SKIN_ALPHA_5      IDM_SKIN+7 
#define IDM_SKIN_ALPHA_6      IDM_SKIN+8 
#define IDM_SKIN_ALPHA_7      IDM_SKIN+9 
#define IDM_SKIN_ALPHA_8      IDM_SKIN+10
#define IDM_SKIN_ALPHA_9      IDM_SKIN+11
#define IDM_SKIN_ALPHA_10     IDM_SKIN+12

