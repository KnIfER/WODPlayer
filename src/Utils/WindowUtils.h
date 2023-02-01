#pragma once
#include <Windows.h>


typedef BOOL (WINAPI *TRANFUNC)(HWND,COLORREF,BYTE,DWORD);

HINSTANCE m_hInstance = NULL;
TRANFUNC  m_pFunc = NULL;
BOOL initialized = FALSE;

BOOL LoadUser32DLL()
{
	BOOL bRet = FALSE;
	if (!initialized)
	{
		m_hInstance = LoadLibrary(L"User32.DLL");
		if(m_hInstance ) 
		{ 
			//取得SetLayeredWindowAttributes函数指针 
			m_pFunc=(TRANFUNC)::GetProcAddress(m_hInstance, "SetLayeredWindowAttributes");
			if(m_pFunc != NULL)
			{
				initialized = TRUE;
				bRet = TRUE;
			}
		}
	}
	return bRet ;
}

BOOL FreeUser32DLL()
{   
	if(m_hInstance != NULL)
	{  
		FreeLibrary(m_hInstance); 
		if(m_pFunc != NULL) //置空
			m_pFunc = NULL ;
		if(initialized != NULL)       //置空
			initialized  = NULL ;
	}
	return TRUE;
}

BOOL ChangeTransparency(HWND hWnd, COLORREF crKey, UINT bAlpha, DWORD dwFlags)
{
	//透明度Sp的值是0-255 
	//其中0为完全透明应该避免出现
	BOOL bRet = FALSE;
	if (m_pFunc == NULL)
		LoadUser32DLL();    

	if(m_pFunc != NULL)
	{
		m_pFunc(hWnd, crKey, bAlpha, dwFlags); 
		bRet = TRUE;   
	}
	return bRet;
}

BOOL SetTransparentWnd(HWND hWnd, COLORREF crKey, UINT cAlpha, BOOL dwFlags)
{
	BOOL bRet = TRUE;   

	//要使使窗体拥有透明效果,首先要有WS_EX_LAYERED扩展属性 WS_EX_LAYERED = 0x80000 
	//SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	//SetWindowLong(hWnd,  GWL_EXSTYLE , GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);  

	if(dwFlags)
	{
		ChangeTransparency(hWnd, crKey, cAlpha, LWA_ALPHA); //设置cAlpha值0-255
	} 
	else
	{
		ChangeTransparency(hWnd, crKey, cAlpha, LWA_COLORKEY);    //crkey相同色变成透明
	}

	return bRet;
}
