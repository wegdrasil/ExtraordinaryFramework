#include <Windows.h>
#include <DirectXMath.h>
#include <cstring>
#include "QuadDemo.h"
#include "WindowSettings.h"
#include "Mesh.h"

#include <cstdio>

void Test();

//--------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{ 
	AllocConsole();
	//--------------------------------------------------------------------------------
	//TEST CODE
	
	Test();

	//--------------------------------------------------------------------------------

	WindowSettings settings;
	settings.m_bFullScreen = true;
	settings.m_iHeight = 400;
	settings.m_iWidth = 400;
	settings.m_sCaption = L"QuadDemo";
	settings.m_bMSAAEnable = false;


	QuadDemo quadDemoApp(hInstance, &settings);

	quadDemoApp.Init();
	
	quadDemoApp.InitializeContent();
	quadDemoApp.InitializeRenderer(&settings, quadDemoApp.GetHWND());
	
	quadDemoApp.Loop();
	
	quadDemoApp.ShutDown();

}
