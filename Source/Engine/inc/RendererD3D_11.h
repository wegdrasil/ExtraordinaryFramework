#pragma once
//--------------------------------------------------------------------------------
#include <d3d11.h>
#include <d3dcompiler.h>
#include "WindowSettings.h"
//--------------------------------------------------------------------------------
class RendererD3D_11
{
public:
	RendererD3D_11();
	~RendererD3D_11();

	void Initialize(WindowSettings* windowSettings, HWND handle);
	void Shutdown();
	void Present();

	void CreateSwapChainAndDevice();
	void CreateInputLayout(ID3D11InputLayout** inputLayout, D3D11_INPUT_ELEMENT_DESC* inputElementDescs, unsigned int numberOfElements, ID3DBlob *blob);
	void CreateRasterizeState(ID3D11RasterizerState** rasterizerState, D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode, bool counterClockwise, bool depthClipEnable);

	void CreateBuffer(ID3D11Buffer** buffer, UINT byteWidth, D3D11_USAGE usage, UINT bindFlags, const void * initData);

	void CompileShader(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR profile, ID3DBlob **blob);

	ID3D11Device* GetDev() { return m_pDevice; }
	ID3D11DeviceContext* GetCtx() { return m_pContext; }
	IDXGISwapChain* GetSwapChain() { return m_pSwapChain; }
	ID3D11RenderTargetView* GetFramebufferRTV() { return m_pRenderTargetView; }
	ID3D11DepthStencilView* GetDepthbufferView() { return  m_pDepthStencilView; }

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

