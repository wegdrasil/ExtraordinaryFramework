#pragma once
#include "Window.h"
#include "RendererD3D_11.h"
//--------------------------------------------------------------------------------
class QuadDemo : public Window
{
	RendererD3D_11 m_Renderer;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

public:
	QuadDemo(HINSTANCE hInstance, WindowSettings *settings) : Window(hInstance, settings) {};

	~QuadDemo() {};
	
	void InitializeRenderer(WindowSettings* windowSettings, HWND handle);
	void PrepareRenderer();
	void Loop();
	void Render();
	
	virtual LRESULT CALLBACK VWindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
};
//--------------------------------------------------------------------------------
