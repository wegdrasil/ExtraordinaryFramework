#pragma once
#include "Window.h"
#include "RendererD3D_11.h"
#include "ConstantBuffer.h"
#include <DirectXMath.h>
#include <vector>

struct Data
{
	DirectX::XMFLOAT2 vec;
};

class AddVecDemo : public Window
{
	RendererD3D_11 m_Renderer;
	
	std::vector<Data> dataA;
	std::vector<Data> dataB;
	
	ID3D11Buffer* m_pInputBufferA;
	ID3D11Buffer* m_pInputBufferB;
	ID3D11Buffer* m_pOutputBuffer;
	ID3D11Buffer* m_pOutputTmpBuffer;

	ID3D11ShaderResourceView* m_pInputA_SRV;
	ID3D11ShaderResourceView* m_pInputB_SRV;
	
	ID3D11UnorderedAccessView* m_pOutputUAV;
	
	ID3D11ComputeShader* m_pComputeShader;

public:

	AddVecDemo(HINSTANCE hInstance, WindowSettings *settings) : Window(hInstance, settings) {};
	~AddVecDemo() {}

	void InitializeRenderer(WindowSettings* windowSettings, HWND handle);
	void InitializeContent();
	void PrepareRenderer();
	void Loop();
	void Render();
	void Work();
	void Quit();
	
	virtual LRESULT CALLBACK VWindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
};
