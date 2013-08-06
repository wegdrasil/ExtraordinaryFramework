#include <Windows.h>
#include "QuadDemo.h"


void Loop()
{
	MSG msg = {0};

	while(WM_QUIT != msg.message)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
		}
	}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{ 
	//Window window(hInstance, &settings);
	
	QuadDemo* demo = new QuadDemo();
	//demo->Go(;)
	demo->m_pWindowSettings->m_bFullScreen = true;
	demo->m_pWindowSettings->m_bMSAAEnable = false;
	demo->m_pWindowSettings->m_iHeight = 600;
	demo->m_pWindowSettings->m_iWidth = 800;
	demo->m_pWindowSettings->m_sCaption = L"eFrame";
		 
	Loop();
	
}