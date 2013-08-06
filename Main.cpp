#include <Windows.h>
#include "Window.h"

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
	WindowSettings settings;
	settings.m_bFullScreen = true;
	settings.m_bMSAAEnable = false;
	settings.m_iHeight = 600;
	settings.m_iWidth = 800;
	settings.m_sCaption = L"eFrame";

	Window window(hInstance, &settings);
	
	window.Init();
	Loop();
	window.ShutDown();
}