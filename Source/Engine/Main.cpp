#include <Windows.h>
#include <DirectXMath.h>
#include <cstring>
#include "AddVectorComputeShaderDemo.h"
#include "WindowSettings.h"
#include "Mesh.h"
#include "Utility.h"

//--------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{ 
	OpenConsole();

	WindowSettings settings;
	settings.m_bFullScreen = true;
	settings.m_iHeight = 600;
	settings.m_iWidth = 600;
	settings.m_sCaption = L"QuadDemo";
	settings.m_bMSAAEnable = true;

	AddVecDemo app(hInstance, &settings);

	app.Init();

	app.InitializeContent();
	app.InitializeRenderer(&settings, app.GetHWND());

	app.Work();

	app.Loop();

	app.Quit();
	app.ShutDown();
}
//--------------------------------------------------------------------------------
