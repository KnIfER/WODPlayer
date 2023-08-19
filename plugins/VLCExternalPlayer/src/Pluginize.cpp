// MIT License
#include <windows.h>
#include <shlwapi.h>
#include <string>

#include "VLCPlayer.h"


struct VWCreateOptions{
	HMODULE hPlugin=0;
	HINSTANCE hHost=0;
	HWND hParent=0;
	LONG_PTR* ret;
};

extern "C" __declspec(dllexport) int vwCreatePlayer(VWCreateOptions VWOpt) 
{
	int error_code=-1;
	VLCPlayer* ret = new VLCPlayer(error_code, VWOpt.hPlugin, VWOpt.hHost, VWOpt.hParent);
	if(VWOpt.ret) *VWOpt.ret = (LONG_PTR)ret;
	return error_code;
}

extern "C" __declspec(dllexport) HWND vwGetHWND(VLCPlayer* player) 
{
	return player->getHWND();
}

extern "C" __declspec(dllexport) void vwRelease(VLCPlayer* player) 
{
	player->Release();
}

extern "C" __declspec(dllexport) void vwPlay(VLCPlayer* player) 
{
	player->Play();
}

extern "C" __declspec(dllexport) void vwPause(VLCPlayer* player) 
{
	player->Pause();
}

extern "C" __declspec(dllexport) void vwStop(VLCPlayer* player) 
{
	player->Stop();
}

extern "C" __declspec(dllexport) bool vwIsPlaying(VLCPlayer* player) 
{
	return player->IsPlaying();
}

extern "C" __declspec(dllexport) bool vwIsPaused(VLCPlayer* player) 
{
	return player->IsPaused();
}

extern "C" __declspec(dllexport) long vwGetPosition(VLCPlayer* player) 
{
	return player->GetPosition();
}

extern "C" __declspec(dllexport) long vwGetDuration(VLCPlayer* player) 
{
	return player->GetDuration();
}

extern "C" __declspec(dllexport) void vwSetPosition(VLCPlayer* player, long pos) 
{
	return player->SetPosition(pos);
}

extern "C" __declspec(dllexport) void vwSetFullScreen(VLCPlayer* player, bool val) 
{
	player->SetFullScreen(val);
}

extern "C" __declspec(dllexport) bool vwPlayVideoFile(VLCPlayer* player, TCHAR* path, CHAR* path1) 
{
	return player->PlayVideoFile(path1);
}

extern "C" __declspec(dllexport) void vwClose(VLCPlayer* player) 
{
	player->Stop();
}


extern "C" __declspec(dllexport) void vwSyncSize(VLCPlayer* player, unsigned int* x, unsigned int* y) 
{
	player->SyncSize(x, y);
}


extern "C" __declspec(dllexport) int vwInterface() 
{
	return 1;
}

extern "C" __declspec(dllexport) float vwSetRate(VLCPlayer* player, float rate) 
{
	return player->SetRate(rate);
}

extern "C" __declspec(dllexport) int vwSetVolume(VLCPlayer* player, int value) 
{
	return player->SetVolume(value);
}