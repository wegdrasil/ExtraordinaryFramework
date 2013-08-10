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
void QuadDemo::InitializeContent()
{
	m_Sphere.CreateMeshFromOBJFile("Content\\Models\\iso.obj");
}
//--------------------------------------------------------------------------------
void QuadDemo::InitializeRenderer(WindowSettings* windowSettings, HWND handle)
{
	m_Renderer.Initialize(windowSettings, handle);
	
	m_Renderer.CreateBuffer(m_pVertexBuffer, sizeof(Vertex) * m_Sphere.m_MeshData.m_vVertices.size(), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, &m_Sphere.m_MeshData.m_vVertices[0]);
	m_Renderer.CreateBuffer(m_pIndexBuffer, sizeof(unsigned int) * m_Sphere.m_MeshData.m_vIndices.size(), D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, &m_Sphere.m_MeshData.m_vIndices[0]);
	
	m_Renderer.CreateRasterizeState(m_pSolidRS, D3D11_FILL_SOLID, D3D11_CULL_BACK, false, true);
	
	ID3D10Blob* VertexShaderBlob;
	ID3D10Blob* PixelShaderBlob;
	m_Renderer.CompileShader(L"VertexShader.hlsl", "VS", "vs_5_0", &VertexShaderBlob);
	m_Renderer.CompileShader(L"PixelShader.hlsl", "PS", "ps_5_0", &PixelShaderBlob);

	m_Renderer.GetDev()->CreateVertexShader(VertexShaderBlob->GetBufferPointer(),  VertexShaderBlob ->GetBufferSize(), nullptr, &m_pVertexShader);
	m_Renderer.GetDev()->CreatePixelShader(PixelShaderBlob ->GetBufferPointer(), PixelShaderBlob ->GetBufferSize(), nullptr, &m_pPixelShader);

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	m_Renderer.CreateInputLayout(m_pInputLayout, vertexDesc, 3, VertexShaderBlob);  

	VertexShaderBlob->Release(); VertexShaderBlob = nullptr;
	PixelShaderBlob->Release(); PixelShaderBlob = nullptr;
}
//--------------------------------------------------------------------------------
void QuadDemo::Render()
{
	float ClearColor[4] = { 0.2f, 0.25f, 0.6f, 1.0f }; 
	m_Renderer.GetCtx()->ClearRenderTargetView(m_Renderer.GetFramebufferRTV(), ClearColor);
	m_Renderer.GetCtx()->ClearDepthStencilView(m_Renderer.GetDepthbufferView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	m_Renderer.GetCtx()->IASetInputLayout(m_pInputLayout);
	m_Renderer.GetCtx()->RSSetState(m_pSolidRS);
	
	m_Renderer.GetCtx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;	
	m_Renderer.GetCtx()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_Renderer.GetCtx()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//ctx->PSSetSamplers(0, 1, &StateLinear);
	//ctx->PSSetShaderResources(0, 1, &texMandrillSRV);

	m_Renderer.GetCtx()->VSSetShader(m_pVertexShader, nullptr, 0);
	m_Renderer.GetCtx()->PSSetShader(m_pPixelShader, nullptr, 0);

	//constant buffer

	m_Renderer.GetCtx()->DrawIndexed(m_Sphere.m_MeshData.m_vIndices.size(), 0, 0);

	m_Renderer.GetSwapChain()->Present(0,0);
}
//--------------------------------------------------------------------------------
