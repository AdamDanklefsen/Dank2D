#include "Graphics.h"
#pragma comment( lib,"d3d11.lib" )


#define DANK_EXCEPTION( hr,note ) Graphics::Exception( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

Graphics::Graphics(HWND hwnd) : hwnd(hwnd) {
	HRESULT hr;
	UINT createFlags = 0u;
#ifdef _DEBUG
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// Swap Chain Param
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = ScreenX;
	sd.BufferDesc.Height = ScreenY;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 1;
	sd.BufferDesc.RefreshRate.Denominator = 60;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	// Swap Chain and D3DDevice
	if (FAILED(hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pImmediateContext))) {
		throw DANK_EXCEPTION(hr, L"Failed Creating Device and Swapchain");
	}
	// Get reference to backbuffer
	if (FAILED(hr = pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(LPVOID*)(&pBackBuffer)))) {
		throw DANK_EXCEPTION(hr, L"Failed Getting Buffer");
	}
	// Set up render target view
	if (FAILED(hr = pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView))) {
		throw DANK_EXCEPTION(hr, L"Failed Creating Render Target View");
	}
	// Set BackBuffer as render target
	pImmediateContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), nullptr);

	// set viewport dimensions
	D3D11_VIEWPORT vp;
	vp.Width = float(ScreenX);
	vp.Height = float(ScreenY);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pImmediateContext->RSSetViewports(1, &vp);

	// Create Textures for CPU RenderTarget
	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = ScreenX;
	sysTexDesc.Height = ScreenY;
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;
	// create the texture
	if (FAILED(hr = pDevice->CreateTexture2D(&sysTexDesc, nullptr, &pSysBufferTexture))) {
		throw DANK_EXCEPTION(hr, L"Creating sysbuffer texture");
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// create the resource view on the texture
	if (FAILED(hr = pDevice->CreateShaderResourceView(pSysBufferTexture.Get(),
		&srvDesc, &pSysBufferTextureView))) {
		throw DANK_EXCEPTION(hr, L"Creating view on sysBuffer texture");
	}

	// Create Pixel Buffer
	if (FAILED(hr = pDevice->CreatePixelShader(
		FramebufferShaders::FramebufferPSBytecode,
		sizeof(FramebufferShaders::FramebufferPSBytecode),
		nullptr,
		&pPixelShader))) {
		throw DANK_EXCEPTION(hr, L"Creating pixel shader");
	}
}
Graphics::~Graphics() {}

Graphics::Exception::Exception(HRESULT hr, const std::wstring & note, const wchar_t * file, unsigned int line) :
	DankException(file, line, note),
	hr(hr) {
}

std::wstring Graphics::Exception::GetErrorName() const {
	return std::wstring();
}

std::wstring Graphics::Exception::GetErrorDescription() const {
	return std::wstring();
}

std::wstring Graphics::Exception::GetFullMessage() const {
	return std::wstring();
}

std::wstring Graphics::Exception::GetExceptionType() const {
	return std::wstring();
}
