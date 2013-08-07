#include <Windows.h>
#include "QuadDemo.h"
#include "WindowSettings.h"
//--------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{ 
	WindowSettings settings;
	settings.m_bFullScreen = true;
	settings.m_iHeight = 400;
	settings.m_iWidth = 400;
	settings.m_sCaption = L"QuadDemo";
	settings.m_bMSAAEnable = false;

	QuadDemo quadDemoApp(hInstance, &settings);
	quadDemoApp.Init();
	quadDemoApp.Loop();
	quadDemoApp.ShutDown();
}
//--------------------------------------------------------------------------------
