#pragma once
//--------------------------------------------------------------------------------
#include <d3d11.h>
#include <d3dcompiler.h>
//--------------------------------------------------------------------------------
#include "IRenderer.h"
//--------------------------------------------------------------------------------
class RendererD3D_11 : public IRenderer
{
public:
	RendererD3D_11();
	virtual ~RendererD3D_11();

	virtual void Initialize(WindowSettings* windowSettings, HWND handle);
	virtual void Shutdown();
	virtual void Present();

	void CreateSwapChainAndDevice();
	void CreateInputLayout(ID3D11InputLayout *inputLayout, D3D11_INPUT_ELEMENT_DESC* inputElementDescs, unsigned int numberOfElements, ID3DBlob *blob);
	void CreateRasterizeState(ID3D11RasterizerState* rasterizerState, D3D11_RASTERIZER_DESC* rsDesc);

	void CompileShader(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR profile, ID3DBlob **blob);

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
	IDXGISwapChain* m_pSwapChain;

	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;

	WindowSettings* m_pWindowSettings;
	HWND m_hWnd;
};
//--------------------------------------------------------------------------------

