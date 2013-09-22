#include <Windows.h>
#include <DirectXMath.h>
#include <cstring>
#include "BlurComputeShaderDemo.h"
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
	settings.m_sCaption = L"ComputeShaderBlurDemo";
	settings.m_bMSAAEnable = false;

	BlurDemo app(hInstance, &settings);

	app.Init();

	app.InitializeContent();
	app.InitializeRenderer(&settings, app.GetHWND());

	app.Loop();

	app.Quit();
	app.ShutDown();
}
//--------------------------------------------------------------------------------
