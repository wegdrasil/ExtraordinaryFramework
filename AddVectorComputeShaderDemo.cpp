#include "AddVectorComputeShaderDemo.h"
#include <iostream>
//--------------------------------------------------------------------------------
void AddVecDemo::Loop()
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
LRESULT CALLBACK AddVecDemo::VWindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
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
void AddVecDemo::InitializeContent()
{
	dataA.resize(64);
	dataB.resize(64);

	for(int i = 0 ; i < 64; i++)
	{
		dataA[i].vec = DirectX::XMFLOAT2(i, i);
		dataB[i].vec = DirectX::XMFLOAT2(i*2, 0.0f);
		
	}

	for(auto i: dataA)
		std::cout << "( " << i.vec.x << ", " << i.vec.y << " )" << std::endl;

	for(auto i: dataB)
		std::cout << "( " << i.vec.x << ", " << i.vec.y << " )" << std::endl;

	std::cout << "Waiting for compute shader" << std::endl;
}
//--------------------------------------------------------------------------------
void AddVecDemo::InitializeRenderer(WindowSettings* windowSettings, HWND handle)
{
	m_Renderer.Initialize(windowSettings, handle);

	auto dev = m_Renderer.GetDev();
	auto ctx = m_Renderer.GetCtx();

	ID3D10Blob* ComputeShaderBlob;
	m_Renderer.CompileShader(L"VecAddComputeShader.hlsl", "main", "cs_5_0", &ComputeShaderBlob);
		
	dev->CreateComputeShader(ComputeShaderBlob->GetBufferPointer(), ComputeShaderBlob->GetBufferSize(), nullptr, &m_pComputeShader);
	ComputeShaderBlob->Release(); ComputeShaderBlob = nullptr;

	D3D11_BUFFER_DESC inputDesc;
	inputDesc.Usage = D3D11_USAGE_DEFAULT;
	inputDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	inputDesc.ByteWidth = sizeof(Data) * 64;
	inputDesc.CPUAccessFlags = 0;
	inputDesc.StructureByteStride = sizeof(Data);
	inputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	D3D11_SUBRESOURCE_DATA initDataDescA;
	initDataDescA.pSysMem = &dataA[0];

	D3D11_SUBRESOURCE_DATA initDataDescB;
	initDataDescB.pSysMem = &dataB[0];

	dev->CreateBuffer(&inputDesc, &initDataDescA, &m_pInputBufferA);
	dev->CreateBuffer(&inputDesc, &initDataDescB, &m_pInputBufferB);

	D3D11_BUFFER_DESC outputDesc;
	outputDesc.Usage = D3D11_USAGE_DEFAULT;
	outputDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	outputDesc.ByteWidth = sizeof(Data) * 64;
	outputDesc.CPUAccessFlags = 0;
	outputDesc.StructureByteStride = sizeof(Data);
	outputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	dev->CreateBuffer(&outputDesc, 0, &m_pOutputBuffer);

	D3D11_BUFFER_DESC outTmpDesc;
	outTmpDesc.Usage = D3D11_USAGE_STAGING;
	outTmpDesc.BindFlags = 0;
	outTmpDesc.ByteWidth = sizeof(Data) * 64;
	outTmpDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	outTmpDesc.StructureByteStride = sizeof(Data);
	outTmpDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	dev->CreateBuffer(&outTmpDesc, 0, &m_pOutputTmpBuffer);
	
	//
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.FirstElement = 0;
	srvDesc.BufferEx.Flags = 0;
	srvDesc.BufferEx.NumElements = 64;

	dev->CreateShaderResourceView(m_pInputBufferA, &srvDesc, &m_pInputA_SRV);
	dev->CreateShaderResourceView(m_pInputBufferB, &srvDesc, &m_pInputB_SRV);

	ID3D11ShaderResourceView* srv[2] = {m_pInputA_SRV, m_pInputB_SRV};

	ctx->CSSetShaderResources(0, 2, srv);
	
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = 0;
	uavDesc.Buffer.NumElements = 64;

	dev->CreateUnorderedAccessView(m_pOutputBuffer, &uavDesc, &m_pOutputUAV);

	ctx->CSSetUnorderedAccessViews(0, 1, &m_pOutputUAV, 0);
}
//--------------------------------------------------------------------------------
void AddVecDemo::Work()
{
	auto ctx = m_Renderer.GetCtx();

	ctx->CSSetShader(m_pComputeShader, nullptr, 0);
	ctx->Dispatch(64, 1, 1);
	
	ctx->CopyResource(m_pOutputTmpBuffer, m_pOutputBuffer);
	
	D3D11_MAPPED_SUBRESOURCE mappedData;
	ctx->Map(m_pOutputTmpBuffer, 0, D3D11_MAP_READ, 0, &mappedData);

	Data* dataView = reinterpret_cast<Data*>(mappedData.pData);

	for(int i = 0; i < 64; i++)
	{
		std::cout << i << "  ( " << dataView[i].vec.x << ", " << dataView[i].vec.y << " )" << std::endl;
	}

	ctx->Unmap(m_pOutputTmpBuffer, 0);

	std::cout << "Done";
}
//--------------------------------------------------------------------------------
void AddVecDemo::Render()
{
	float ClearColor[4] = { 0.2f, 0.25f, 0.6f, 1.0f };

	m_Renderer.GetCtx()->ClearRenderTargetView(m_Renderer.GetFramebufferRTV(), ClearColor);

	m_Renderer.GetSwapChain()->Present(0,0);
}
//--------------------------------------------------------------------------------
void AddVecDemo::Quit()
{	
	m_pInputBufferA->Release(); m_pInputBufferA = nullptr;
	m_pInputBufferB->Release(); m_pInputBufferB = nullptr;
	m_pOutputBuffer->Release(); m_pOutputBuffer = nullptr;
	m_pOutputTmpBuffer->Release(); m_pOutputTmpBuffer = nullptr;

	m_pInputA_SRV->Release(); m_pInputA_SRV = nullptr;
	m_pInputB_SRV->Release(); m_pInputB_SRV = nullptr;
	
	m_pOutputUAV->Release(); m_pOutputUAV = nullptr;
	
	m_pComputeShader->Release(); m_pComputeShader = nullptr;

	m_Renderer.Shutdown();
}
//--------------------------------------------------------------------------------
