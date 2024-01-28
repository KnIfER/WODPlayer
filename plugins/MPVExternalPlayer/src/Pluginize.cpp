// MIT License
#include <windows.h>
#include <shlwapi.h>
#include <string>

#include "MPVPlayer.h"


struct VWCreateOptions{
	HMODULE hPlugin=0;
	HINSTANCE hHost=0;
	HWND hParent=0;
	LONG_PTR* ret;
};

extern "C" __declspec(dllexport) int vwCreatePlayer(VWCreateOptions VWOpt) 
{
	int error_code=-1;
	MPVPlayer* ret = new MPVPlayer(error_code, VWOpt.hPlugin, VWOpt.hHost, VWOpt.hParent);
	if(VWOpt.ret) *VWOpt.ret = (LONG_PTR)ret;
	return error_code;
}

extern "C" __declspec(dllexport) HWND vwGetHWND(MPVPlayer* player) 
{
	return player->getHWND();
}

extern "C" __declspec(dllexport) void vwRelease(MPVPlayer* player) 
{
	player->Release();
}

extern "C" __declspec(dllexport) void vwPlay(MPVPlayer* player) 
{
	player->Play();
}

extern "C" __declspec(dllexport) void vwPause(MPVPlayer* player) 
{
	player->Pause();
}

extern "C" __declspec(dllexport) void vwStop(MPVPlayer* player) 
{
	player->Stop();
}

extern "C" __declspec(dllexport) bool vwIsPlaying(MPVPlayer* player) 
{
	return player->IsPlaying();
}

extern "C" __declspec(dllexport) bool vwIsPaused(MPVPlayer* player) 
{
	return player->IsPaused();
}

extern "C" __declspec(dllexport) long vwGetPosition(MPVPlayer* player) 
{
	return player->GetPosition();
}

extern "C" __declspec(dllexport) long vwGetDuration(MPVPlayer* player) 
{
	return player->GetDuration();
}

extern "C" __declspec(dllexport) void vwSetPosition(MPVPlayer* player, long pos) 
{
	return player->SetPosition(pos);
}

extern "C" __declspec(dllexport) void vwSetFullScreen(MPVPlayer* player, bool val) 
{
	player->SetFullScreen(val);
}

extern "C" __declspec(dllexport) bool vwPlayVideoFile(MPVPlayer* player, TCHAR* path, CHAR* path1) 
{
	return player->PlayVideoFile(path1);
}

extern "C" __declspec(dllexport) void vwClose(MPVPlayer* player) 
{
	player->Stop();
}


extern "C" __declspec(dllexport) void vwSyncSize(MPVPlayer* player, unsigned int* x, unsigned int* y) 
{
	player->SyncSize(x, y);
}


extern "C" __declspec(dllexport) int vwInterface() 
{
	return 1;
}

extern "C" __declspec(dllexport) float vwSetRate(MPVPlayer* player, float rate) 
{
	return player->SetRate(rate);
}

extern "C" __declspec(dllexport) int vwSetVolume(MPVPlayer* player, int value) 
{
	return player->SetVolume(value);
}


extern "C" __declspec(dllexport) void vwSetRotation(MPVPlayer* player, int value) 
{
	player->SetRotation(value);
}


extern "C" __declspec(dllexport) int64_t vwGetRotation(MPVPlayer* player) 
{
	return player->GetRotation();
}


