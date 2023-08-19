
#pragma once

#define WOD_COPYDATA 1



__declspec(selectany) std::vector<std::wstring> _args;
__declspec(selectany) bool isClosing = false;

void parseCommandLine(const TCHAR* commandLine, std::vector<std::wstring>& arguments);