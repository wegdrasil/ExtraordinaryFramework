#include "BlurComputeShaderDemo.h"
//--------------------------------------------------------------------------------
void BlurDemo::Loop()
{
	MSG msg = { 0 };

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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
LRESULT CALLBACK BlurDemo::VWindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
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
void BlurDemo::InitializeContent()
{
	m_Mesh.GenQuad(1.0f);
	
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
		
}
//--------------------------------------------------------------------------------
void BlurDemo::InitializeRenderer(WindowSettings* windowSettings, HWND handle)
{
	m_Renderer.Initialize(windowSettings, handle);
	ID3D11Device* dev = m_Renderer.GetDev();

	m_Renderer.CreateBuffer(&m_pVertexBuffer, sizeof(Vertex)* m_Mesh.m_MeshData.m_vVertices.size(),
		D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, &m_Mesh.m_MeshData.m_vVertices[0]);
	m_Renderer.CreateBuffer(&m_pIndexBuffer, sizeof(unsigned int)* m_Mesh.m_MeshData.m_vIndices.size(),
		D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, &m_Mesh.m_MeshData.m_vIndices[0]);

	cbPO_VS.InitConstBuffer(dev);

	m_Renderer.CreateRasterizeState(&m_pSolidRS, D3D11_FILL_SOLID, D3D11_CULL_BACK, false, true);

	ID3D10Blob* VertexShaderBlob;
	ID3D10Blob* PixelShaderBlob;
	ID3D10Blob* HorizontalBlurShaderBlob;
	ID3D10Blob* VerticalBlurShaderBlob;

	m_Renderer.CompileShader(L"Source\\Shaders\\BlurVertexShader.hlsl", "main", "vs_5_0", &VertexShaderBlob);
	m_Renderer.CompileShader(L"Source\\Shaders\\BlurPixelShader.hlsl", "main", "ps_5_0", &PixelShaderBlob);
	m_Renderer.CompileShader(L"Source\\Shaders\\HorizontalBlurComputeShader.hlsl", "main", "cs_5_0", &HorizontalBlurShaderBlob);
	m_Renderer.CompileShader(L"Source\\Shaders\\VerticalBlurComputeShader.hlsl", "main", "cs_5_0", &VerticalBlurShaderBlob);

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_Renderer.CreateInputLayout(&m_pInputLayout, vertexDesc, 3, VertexShaderBlob);

	dev->CreateVertexShader(VertexShaderBlob->GetBufferPointer(), VertexShaderBlob->GetBufferSize(), nullptr, &m_pVertexShader);
	dev->CreatePixelShader(PixelShaderBlob->GetBufferPointer(), PixelShaderBlob->GetBufferSize(), nullptr, &m_pPixelShader);
	dev->CreateComputeShader(HorizontalBlurShaderBlob->GetBufferPointer(), HorizontalBlurShaderBlob->GetBufferSize(), nullptr, &m_pHorizontalBlurCS);
	dev->CreateComputeShader(VerticalBlurShaderBlob->GetBufferPointer(), VerticalBlurShaderBlob->GetBufferSize(), nullptr, &m_pVerticalBlurCS);

	VertexShaderBlob->Release(); VertexShaderBlob = nullptr;
	PixelShaderBlob->Release(); PixelShaderBlob = nullptr;
	HorizontalBlurShaderBlob->Release(); HorizontalBlurShaderBlob = nullptr;
	VerticalBlurShaderBlob->Release(); VerticalBlurShaderBlob = nullptr;

	m_Renderer.CreateTextureFromDDSFile(L"Assets\\Textures\\mandrill.dds", &m_pTex, &m_pTexSRV, &m_pSamplerState);

	m_Renderer.CreateOffscreenTexture(&m_pOffscreenRTV, &m_pOffscreenSRV, &m_pOffscreenUAV);
	PrepareBlurTexture();
}
//--------------------------------------------------------------------------------
void BlurDemo::PrepareBlurTexture()
{
	ID3D11Device* dev = m_Renderer.GetDev();

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = 600;
	texDesc.Height = 600;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* blurredTexture = nullptr;
	dev->CreateTexture2D(&texDesc, nullptr, &blurredTexture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	
	dev->CreateShaderResourceView(blurredTexture, &srvDesc, &m_pBluredTexSRV);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;

	dev->CreateUnorderedAccessView(blurredTexture, &uavDesc, &m_pBluredTexUAV);
}
//--------------------------------------------------------------------------------
void BlurDemo::Update()
{
	float x = 0.007f;
	if (GetAsyncKeyState('W'))
		CamY += x;
	if (GetAsyncKeyState('S'))
		CamY -= x;
	if (GetAsyncKeyState('D'))
		CamX += x;
	if (GetAsyncKeyState('A'))
		CamX -= x;
	if (GetAsyncKeyState('X'))
		CamZ += x;
	if (GetAsyncKeyState('Z'))
		CamZ -= x;

	SetCamera(CamX, CamY, CamZ);
}
//--------------------------------------------------------------------------------
void BlurDemo::Render()
{
	ID3D11DeviceContext* ctx = m_Renderer.GetCtx();

	float ClearColor[4] = { 0.2f, 0.25f, 0.6f, 1.0f };
	
	////////////////////////////////////////////////////////////////////////////////
	//Rendering to offscreen texture

	ctx->OMSetRenderTargets(1, &m_pOffscreenRTV, m_Renderer.GetDepthbufferView());

	ctx->ClearRenderTargetView(m_pOffscreenRTV, ClearColor);
	ctx->ClearDepthStencilView(m_Renderer.GetDepthbufferView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ctx->IASetInputLayout(m_pInputLayout);
	ctx->RSSetState(m_pSolidRS);

	ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ctx->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	ctx->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	
	ctx->PSSetSamplers(0, 1, &m_pSamplerState);
	ctx->PSSetShaderResources(0, 1, &m_pTexSRV);

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
	////////////////////////////////////////////////////////////////////////////////
	//Compute shader works now

	m_Renderer.SetRenderTarget();
		
	ID3D11ShaderResourceView* nullSRV[1] = { 0 };
	ID3D11UnorderedAccessView* nullUAV[1] = { 0 };
    
	//horizontal pass
	ctx->CSSetShaderResources(0, 1, &m_pOffscreenSRV);
	ctx->CSSetUnorderedAccessViews(0, 1, &m_pBluredTexUAV, 0);

	ctx->CSSetShader(m_pHorizontalBlurCS, nullptr, 0);
	ctx->Dispatch(1, 600, 1);
	
	ctx->CSSetShaderResources(0, 1, nullSRV);
	ctx->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	
	ctx->CSSetShader(0, 0, 0);

	//vertical pass
	ctx->CSSetShaderResources(0, 1, &m_pBluredTexSRV);
	ctx->CSSetUnorderedAccessViews(0, 1, &m_pOffscreenUAV, 0);

	ctx->CSSetShader(m_pVerticalBlurCS, nullptr, 0);
	ctx->Dispatch(600, 1, 1);

	ctx->CSSetShaderResources(0, 1, nullSRV);
	ctx->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);

	/////////////////////////////////////////////////////////////////////////////////////////
	//Render blurred

	float ClearColor2[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
		
	ctx->ClearRenderTargetView(m_Renderer.GetFramebufferRTV(), ClearColor2);
	ctx->ClearDepthStencilView(m_Renderer.GetDepthbufferView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	ctx->PSSetShaderResources(0, 1, &m_pOffscreenSRV);
	ctx->PSSetShader(m_pPixelShader, nullptr, 0);

	ctx->DrawIndexed(m_Mesh.m_MeshData.m_vIndices.size(), 0, 0);
	////////////////////////////////////////////////////////////////////////////////

	m_Renderer.GetSwapChain()->Present(0, 0);
}
//--------------------------------------------------------------------------------
void BlurDemo::SetOrthProjMat(float x)
{
	float ratio = 1.0f;

	//DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicOffCenterLH(-x*ratio, x*ratio, -x*ratio, x*ratio, -x, x);  
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(3.1415 / 4, 1.0f, 1.0f, 50.0f);
	DirectX::XMStoreFloat4x4(&m_Proj, P);

	//reset viewport
}
//--------------------------------------------------------------------------------
void BlurDemo::SetCamera(float _x, float _y, float _z)
{
	float x = _z*sinf(_x)*cosf(_y);
	float z = _z*sinf(_x)*sinf(_y);
	float y = _z*cosf(_x);

	DirectX::XMVECTOR pos = DirectX::XMVectorSet(x, y, z, 1.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(pos, target, up);
	DirectX::XMStoreFloat4x4(&m_View, V);
}
//--------------------------------------------------------------------------------
void BlurDemo::Quit()
{
	m_pOffscreenRTV->Release(); m_pOffscreenRTV = nullptr;
	m_pOffscreenSRV->Release(); m_pOffscreenSRV = nullptr;
	m_pOffscreenUAV->Release(); m_pOffscreenUAV = nullptr;

	m_pBluredTexSRV->Release(); m_pBluredTexSRV = nullptr;
	m_pBluredTexUAV->Release(); m_pBluredTexUAV = nullptr;

	m_pHorizontalBlurCS->Release(); m_pHorizontalBlurCS = nullptr;
	m_pVerticalBlurCS->Release(); m_pVerticalBlurCS = nullptr;

	m_pPixelShader->Release(); m_pPixelShader = nullptr;
	m_pVertexShader->Release(); m_pVertexShader = nullptr;
	m_pInputLayout->Release(); m_pInputLayout = nullptr;
	m_pSolidRS->Release(); m_pSolidRS = nullptr;
	m_pIndexBuffer->Release(); m_pIndexBuffer = nullptr;
	m_pVertexBuffer->Release(); m_pVertexBuffer = nullptr;

	m_Renderer.Shutdown();
}
//--------------------------------------------------------------------------------
