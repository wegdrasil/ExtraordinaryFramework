#pragma once
#include "Window.h"
#include "RendererD3D_11.h"
#include "ConstantBuffer.h"
#include "Mesh.h"

//--------------------------------------------------------------------------------
struct cbPerObjectVSBlur
{
	DirectX::XMFLOAT4X4 WorldViewProj;
};
//--------------------------------------------------------------------------------
class BlurDemo : public Window
{
	RendererD3D_11 m_Renderer;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11InputLayout* m_pInputLayout;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11RasterizerState *m_pSolidRS;

	ID3D11Resource *m_pTex;
	ID3D11ShaderResourceView *m_pTexSRV;
	ID3D11SamplerState *m_pSamplerState;

	Mesh m_Mesh;
	Mesh m_Quad;
	ConstBuffer<cbPerObjectVSBlur> cbPO_VS;

	DirectX::XMFLOAT4X4 m_World;
	DirectX::XMFLOAT4X4 m_View;
	DirectX::XMFLOAT4X4 m_Proj;

	float CamX, CamY, CamZ;

	//
	ID3D11RenderTargetView* m_pOffscreenRTV;
	ID3D11ShaderResourceView* m_pOffscreenSRV;
	ID3D11UnorderedAccessView* m_pOffscreenUAV;

public:
	BlurDemo(HINSTANCE hInstance, WindowSettings *settings) : Window(hInstance, settings) {};

	~BlurDemo() {};

	void InitializeRenderer(WindowSettings* windowSettings, HWND handle);
	void InitializeContent();
	void PrepareRenderer();
	
	void Loop();
	void Render();
	void Update();
	void Quit();

	void SetOrthProjMat(float x);
	void SetCamera(float x, float y, float z);
	
	virtual LRESULT CALLBACK VWindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
};

//--------------------------------------------------------------------------------
