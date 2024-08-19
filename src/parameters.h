
#pragma once

#define WOD_COPYDATA 1



__declspec(selectany) std::vector<std::wstring> _args;
__declspec(selectany) bool isClosing = false;


//__declspec(selectany) QkString mLockStr;
__declspec(selectany) QkString readyT = L"READY:";
__declspec(selectany) HANDLE hMutexTemp;
__declspec(selectany) HANDLE hMutexReady;
__declspec(selectany) bool deleting;

void parseCommandLine(const TCHAR* commandLine, std::vector<std::wstring>& arguments);