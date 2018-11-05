#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "DankException.h"
#include "Colors.h"
#include <wrl.h>

class Graphics {
public:
	class Exception : public DankException {
	public:
		Exception(HRESULT hr, const std::wstring& note, const wchar_t* file, unsigned int line);
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
public:
	Graphics(HWND hwnd);
	~Graphics();
	Graphics& operator=(const Graphics&) = delete;
	void EndFrame();
	void BeginFrame();
	void Pixel(int x, int y, Color c);

	Microsoft::WRL::ComPtr<IDXGISwapChain>					pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>					pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>				pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11Resource>					pBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>			pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>					pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>				pPixelShader;

private:
	HWND hwnd;
	static int constexpr ScreenX = 800;
	static int constexpr ScreenY = 600;
};

