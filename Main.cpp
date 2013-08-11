#include <Windows.h>
#include <DirectXMath.h>
#include <cstring>
#include "QuadDemo.h"
#include "WindowSettings.h"
#include "Mesh.h"
//--------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{ 
	AllocConsole();
	
	WindowSettings settings;
	settings.m_bFullScreen = true;
	settings.m_iHeight = 600;
	settings.m_iWidth = 600;
	settings.m_sCaption = L"QuadDemo";
	settings.m_bMSAAEnable = false;


	QuadDemo quadDemoApp(hInstance, &settings);

	quadDemoApp.Init();
	
	quadDemoApp.InitializeContent();
	quadDemoApp.InitializeRenderer(&settings, quadDemoApp.GetHWND());
	
	quadDemoApp.Loop();
	
	quadDemoApp.ShutDown();
}
//--------------------------------------------------------------------------------
