// MIT License
#include <windows.h>
#include <shlwapi.h>
#include <string>

#include "MFPlayer2.h"


struct VWCreateOptions{
	HMODULE hPlugin=0;
	HINSTANCE hHost=0;
	HWND hParent=0;
	LONG_PTR* ret;
};

extern "C" __declspec(dllexport) int vwCreatePlayer(VWCreateOptions VWOpt) 
{
	int error_code=-1;
	MFPlayer2* ret = new MFPlayer2(error_code, VWOpt.hHost, VWOpt.hParent);
	if(VWOpt.ret) *VWOpt.ret = (LONG_PTR)ret;
	return error_code;
}

extern "C" __declspec(dllexport) HWND vwGetHWND(MFPlayer2* player) 
{
	return player->getHWND();
}

extern "C" __declspec(dllexport) void vwRelease(MFPlayer2* player) 
{
	player->DoRelease();
}

extern "C" __declspec(dllexport) void vwPlay(MFPlayer2* player) 
{
	player->Play();
}

extern "C" __declspec(dllexport) void vwPause(MFPlayer2* player) 
{
	player->Pause();
}

extern "C" __declspec(dllexport) void vwStop(MFPlayer2* player) 
{
	player->Stop();
}

extern "C" __declspec(dllexport) bool vwIsPlaying(MFPlayer2* player) 
{
	return player->IsPlaying();
}

extern "C" __declspec(dllexport) bool vwIsPaused(MFPlayer2* player) 
{
	return player->IsPaused();
}

extern "C" __declspec(dllexport) long vwGetPosition(MFPlayer2* player) 
{
	return player->GetPosition()/10000;
}

extern "C" __declspec(dllexport) long vwGetDuration(MFPlayer2* player) 
{
	return player->GetDuration()/10000;
}

extern "C" __declspec(dllexport) void vwSetPosition(MFPlayer2* player, long pos) 
{
	return player->SetPosition(pos*10000);
}

extern "C" __declspec(dllexport) void vwSetFullScreen(MFPlayer2* player, bool val) 
{
	player->SetFullScreen(val);
}

extern "C" __declspec(dllexport) bool vwPlayVideoFile(MFPlayer2* player, TCHAR* path) 
{
	return player->PlayVideoFile(path);
}

extern "C" __declspec(dllexport) void vwClose(MFPlayer2* player) 
{
	player->Stop();
}


extern "C" __declspec(dllexport) void vwSyncSize(MFPlayer2* player, unsigned int* x, unsigned int* y) 
{
	player->SyncSize(x, y);
}


extern "C" __declspec(dllexport) int vwInterface() 
{
	return 1;
}
