#include "QuadDemo.h"
//--------------------------------------------------------------------------------
void QuadDemo::Loop()
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
			Render();
		}
	}
}
//--------------------------------------------------------------------------------
LRESULT CALLBACK QuadDemo::VWindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_ESCAPE:
				ShutDown();
				break;
			}
			break;
		}
	}
	return Window::VWindowProc(hWnd, uiMsg, wParam, lParam);
}
//--------------------------------------------------------------------------------
void QuadDemo::InitializeRenderer(WindowSettings* windowSettings, HWND handle)
{
	m_Renderer.Initialize(windowSettings, handle);
}
//--------------------------------------------------------------------------------
void QuadDemo::Render()
{
	float ClearColor[4] = { 1.0f, 0.5f, 0.25f, 1.0f }; 
	m_Renderer.GetCtx()->ClearRenderTargetView(m_Renderer.GetFramebufferRTV(), ClearColor);
	m_Renderer.GetCtx()->ClearDepthStencilView(m_Renderer.GetDepthbufferView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	m_Renderer.GetSwapChain()->Present(0,0);
}
//--------------------------------------------------------------------------------
