#pragma once
#include "Window.h"
#include "RendererD3D_11.h"
#include "Mesh.h"

//--------------------------------------------------------------------------------
class QuadDemo : public Window
{
	RendererD3D_11 m_Renderer;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11InputLayout* m_pInputLayout;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11RasterizerState *m_pSolidRS;

	Mesh m_Sphere; 

public:
	QuadDemo(HINSTANCE hInstance, WindowSettings *settings) : Window(hInstance, settings) {};

	~QuadDemo() {};
	
	void InitializeRenderer(WindowSettings* windowSettings, HWND handle);
	void InitializeContent();
	void PrepareRenderer();
	void Loop();
	void Render();
	
	virtual LRESULT CALLBACK VWindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
};
//--------------------------------------------------------------------------------
