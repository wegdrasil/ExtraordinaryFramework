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
			Update();
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
	m_Mesh.GenQuad(1.0f);
	//m_Mesh.CreateMeshFromOBJFile("Content\\Models\\head.obj");
	//m_Mesh.GenerateNormals();

	//matrix wvp
	DirectX::XMMATRIX I = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&m_World, I);
	DirectX::XMStoreFloat4x4(&m_View, I);
	DirectX::XMStoreFloat4x4(&m_Proj, I);

	//camera
	CamX = 0.0f;
	CamY = 0.0f;
	CamZ = -1.9f;
	SetOrthProjMat(4.0f);
	//lights
}
//--------------------------------------------------------------------------------
void QuadDemo::InitializeRenderer(WindowSettings* windowSettings, HWND handle)
{
	m_Renderer.Initialize(windowSettings, handle);
	
	m_Renderer.CreateBuffer(&m_pVertexBuffer, sizeof(Vertex) * m_Mesh.m_MeshData.m_vVertices.size(), 
		D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, &m_Mesh.m_MeshData.m_vVertices[0]);
	m_Renderer.CreateBuffer(&m_pIndexBuffer, sizeof(unsigned int) * m_Mesh.m_MeshData.m_vIndices.size(), 
		D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, &m_Mesh.m_MeshData.m_vIndices[0]);
	
	cbPO_VS.InitConstBuffer(m_Renderer.GetDev());

	m_Renderer.CreateRasterizeState(&m_pSolidRS, D3D11_FILL_SOLID, D3D11_CULL_BACK, false, true);
	
	ID3D10Blob* VertexShaderBlob;
	ID3D10Blob* PixelShaderBlob;
	m_Renderer.CompileShader(L"QuadVertexShader.hlsl", "VS", "vs_5_0", &VertexShaderBlob);
	m_Renderer.CompileShader(L"QuadPixelShader.hlsl", "PS", "ps_5_0", &PixelShaderBlob);
	
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	m_Renderer.CreateInputLayout(&m_pInputLayout, vertexDesc, 3, VertexShaderBlob);
	m_Renderer.GetDev()->CreateVertexShader(VertexShaderBlob->GetBufferPointer(),  VertexShaderBlob ->GetBufferSize(), nullptr, &m_pVertexShader);
	m_Renderer.GetDev()->CreatePixelShader(PixelShaderBlob ->GetBufferPointer(), PixelShaderBlob ->GetBufferSize(), nullptr, &m_pPixelShader);

	VertexShaderBlob->Release(); VertexShaderBlob = nullptr;
	PixelShaderBlob->Release(); PixelShaderBlob = nullptr;
}
//--------------------------------------------------------------------------------
void QuadDemo::Update()
{
	float x = 0.007f;
	if(GetAsyncKeyState('W'))
		CamY += x;
	if(GetAsyncKeyState('S'))
		CamY -= x;
	if(GetAsyncKeyState('D'))
		CamX += x;
	if(GetAsyncKeyState('A'))
		CamX -= x;
	if(GetAsyncKeyState('X'))
		CamZ += x;
	if(GetAsyncKeyState('Z'))
		CamZ -= x;

	SetCamera(CamX, CamY, CamZ);
}
//--------------------------------------------------------------------------------
void QuadDemo::Render()
{
	ID3D11DeviceContext* ctx = m_Renderer.GetCtx();

	float ClearColor[4] = { 0.2f, 0.25f, 0.6f, 1.0f };

	ctx->ClearRenderTargetView(m_Renderer.GetFramebufferRTV(), ClearColor);
	ctx->ClearDepthStencilView(m_Renderer.GetDepthbufferView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	ctx->IASetInputLayout(m_pInputLayout);
	ctx->RSSetState(m_pSolidRS);
	
	ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;	
	ctx->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	ctx->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//ctx->PSSetSamplers(0, 1, &StateLinear);
	//ctx->PSSetShaderResources(0, 1, &texMandrillSRV);

	ctx->VSSetShader(m_pVertexShader, nullptr, 0);
	ctx->PSSetShader(m_pPixelShader, nullptr, 0);

	//constant buffer
	DirectX::XMMATRIX w = DirectX::XMLoadFloat4x4(&m_World);
	DirectX::XMMATRIX v = DirectX::XMLoadFloat4x4(&m_View);
	DirectX::XMMATRIX p = DirectX::XMLoadFloat4x4(&m_Proj);
	
	DirectX::XMStoreFloat4x4(&cbPO_VS.data.WorldViewProj, DirectX::XMMatrixTranspose(v*p));
	cbPO_VS.UpdateConstBuffer(m_Renderer.GetCtx());
	ID3D11Buffer *tmp = cbPO_VS.GetBuffer();
	ctx->VSSetConstantBuffers(0, 1, &tmp);

	ctx->DrawIndexed(m_Mesh.m_MeshData.m_vIndices.size(), 0, 0);
	
	m_Renderer.GetSwapChain()->Present(0,0);
}
//--------------------------------------------------------------------------------
void QuadDemo::SetOrthProjMat(float x)
{   
	float ratio = 1.0f;

	//DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicOffCenterLH(-x*ratio, x*ratio, -x*ratio, x*ratio, -x, x);  
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(3.1415/4, 1.0f, 1.0f, 50.0f);
	DirectX::XMStoreFloat4x4(&m_Proj, P);

	//reset viewport
}
//--------------------------------------------------------------------------------
void QuadDemo::SetCamera(float _x, float _y, float _z)
{
	float x = _z*sinf(_x)*cosf(_y);
	float z = _z*sinf(_x)*sinf(_y);
	float y = _z*cosf(_x);

	DirectX::XMVECTOR pos    = DirectX::XMVectorSet(x, y, z, 1.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR up     = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(pos, target, up);
	DirectX::XMStoreFloat4x4(&m_View, V);
}
//--------------------------------------------------------------------------------
void QuadDemo::Quit()
{
	m_pPixelShader->Release(); m_pPixelShader = nullptr;
	m_pVertexShader->Release(); m_pVertexShader = nullptr;
	m_pInputLayout->Release(); m_pInputLayout = nullptr;
	m_pSolidRS->Release(); m_pSolidRS = nullptr;
	m_pIndexBuffer->Release(); m_pIndexBuffer = nullptr;
	m_pVertexBuffer->Release();m_pVertexBuffer = nullptr;

	m_Renderer.Shutdown();
}