#include "RendererD3D_11.h"
//--------------------------------------------------------------------------------
RendererD3D_11::RendererD3D_11()
{
}
//--------------------------------------------------------------------------------
RendererD3D_11::~RendererD3D_11()
{
}
//--------------------------------------------------------------------------------
void RendererD3D_11::Initialize(WindowSettings* windowSettings, HWND handle)
{
	m_pWindowSettings = windowSettings;
	m_hWnd = handle;

	CreateSwapChainAndDevice();
}
//--------------------------------------------------------------------------------
void RendererD3D_11::CreateSwapChainAndDevice()
{
	unsigned int createDeviceFlags = 0;
#if defined (DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	D3D11CreateDevice(
		nullptr, //primary display adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
		nullptr, // wybierze najwy¿szy mo¿liwie dostêpny feature level 
		0,  
		D3D11_SDK_VERSION,
		&m_pDevice,
		&featureLevel, // zwróci najwy¿szy mo¿liwie dostêpny feature level
		&m_pContext);

	if(featureLevel !=  D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(nullptr, L"Direct3D Feature Level 11 unsupported.", 0, 0);
	}

	unsigned int MSAAQuality;
	m_pDevice ->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &MSAAQuality);

	DXGI_SWAP_CHAIN_DESC scDesc;
	ZeroMemory(&scDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	scDesc.BufferDesc.Width = m_pWindowSettings->m_iWidth;
	scDesc.BufferDesc.Height = m_pWindowSettings->m_iHeight;
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;
	scDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if(m_pWindowSettings->m_bMSAAEnable)
	{
		scDesc.SampleDesc.Count = 4;
		scDesc.SampleDesc.Quality = MSAAQuality - 1;
	}
	else
	{
		scDesc.SampleDesc.Count = 1;
		scDesc.SampleDesc.Quality = 0;
	}

	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 1;
	scDesc.OutputWindow = m_hWnd;
	scDesc.Windowed = m_pWindowSettings->m_bFullScreen;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Flags = 0;

	//swap chain mo¿na stworzyæ za pomoc¹ interfejsu IDXGIFactory, ale aby otrzymaæ ten poprawny interfejs,
	//nale¿y pobraæ go po przez deviced3d11, nastêpnie dxgidevice i dxgiadapter
	IDXGIDevice *dxgiDevice = nullptr;
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter *dxgiAdapter = nullptr;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory *dxgiFactory = nullptr;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	dxgiFactory->CreateSwapChain(m_pDevice, &scDesc, &m_pSwapChain);

	dxgiFactory ->Release(); dxgiFactory = nullptr;
	dxgiAdapter ->Release(); dxgiAdapter = nullptr;
	dxgiDevice ->Release(); dxgiDevice = nullptr;

	//tworzenie render target view
	ID3D11Texture2D *backBuffer;
	m_pSwapChain ->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	m_pDevice ->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView);
	backBuffer ->Release(); backBuffer = nullptr;

	//depth/stencil buffer and view
	D3D11_TEXTURE2D_DESC depthstencilDesc;
	depthstencilDesc.Width = m_pWindowSettings->m_iWidth;
	depthstencilDesc.Height = m_pWindowSettings->m_iHeight;
	depthstencilDesc.MipLevels = 1;
	depthstencilDesc.ArraySize = 1;
	depthstencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if(m_pWindowSettings->m_bMSAAEnable)
	{
		depthstencilDesc.SampleDesc.Count = 4;
		depthstencilDesc.SampleDesc.Quality = MSAAQuality - 1;
	}
	else
	{
		depthstencilDesc.SampleDesc.Count = 1;
		depthstencilDesc.SampleDesc.Quality = 0;
	}
	depthstencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthstencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthstencilDesc.CPUAccessFlags = 0;
	depthstencilDesc.MiscFlags = 0;

	ID3D11Texture2D *depthstencilBuffer;
	m_pDevice ->CreateTexture2D(&depthstencilDesc, 0, &depthstencilBuffer);
	m_pDevice ->CreateDepthStencilView(depthstencilBuffer, 0, &m_pDepthStencilView);

	m_pContext ->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//viewport
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = m_pWindowSettings->m_iWidth;
	vp.Height = m_pWindowSettings->m_iHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_pContext ->RSSetViewports(1, &vp);
}
//--------------------------------------------------------------------------------
void RendererD3D_11::CompileShader(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR profile, ID3DBlob **blob)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *shaderBlob = nullptr;
	ID3DBlob *errBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(srcFile, 0, 0, entryPoint, profile, flags, 0, &shaderBlob, &errBlob);

	if(FAILED(hr))
	{
		//DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DCompileFromFile", true);
		
		if (errBlob != nullptr)
			errBlob->Release();
		errBlob = nullptr;

		if (shaderBlob != nullptr)
			shaderBlob->Release();
		shaderBlob = nullptr;

		throw std::exception("Nie uda³o siê skompilowaæ shadera z pliku!");
	}

	*blob = shaderBlob;
}
//--------------------------------------------------------------------------------
void RendererD3D_11::CreateInputLayout(ID3D11InputLayout *inputLayout, D3D11_INPUT_ELEMENT_DESC* inputElementDescs, unsigned int numberOfElements, ID3DBlob *blob)
{
	m_pDevice->CreateInputLayout(inputElementDescs, numberOfElements, blob ->GetBufferPointer(), blob ->GetBufferSize(), &inputLayout);
}
//--------------------------------------------------------------------------------
void RendererD3D_11::CreateRasterizeState(ID3D11RasterizerState* rasterizerState, D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode, bool counterClockwise, bool depthClipEnable)
{
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = fillMode;
	rsDesc.CullMode = cullMode;
	rsDesc.FrontCounterClockwise = counterClockwise;
	rsDesc.DepthClipEnable = depthClipEnable;

	m_pDevice ->CreateRasterizerState(&rsDesc, &rasterizerState);
}
//--------------------------------------------------------------------------------
void RendererD3D_11::CreateBuffer(ID3D11Buffer* buffer, UINT byteWidth, D3D11_USAGE usage, UINT bindFlags, const void * initData)
{
	D3D11_BUFFER_DESC vbDesc;
	vbDesc.ByteWidth = byteWidth;
	vbDesc.Usage = usage;
	vbDesc.BindFlags = bindFlags;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertInitDesc;
	vertInitDesc.pSysMem = initData;

	m_pDevice->CreateBuffer(&vbDesc, &vertInitDesc, &buffer);
}
//--------------------------------------------------------------------------------
void RendererD3D_11::Present()
{
}
//--------------------------------------------------------------------------------
void RendererD3D_11::Shutdown()
{
}
//--------------------------------------------------------------------------------