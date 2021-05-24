#pragma once
#include "CSingleton.h"
class CDevice : public CSingleton<CDevice>
{
	SINGLETON(CDevice)

private:
	// Swap Chain Property
	DXGI_RATIONAL m_tDefaultRefreshRate;		// 기본 모니터 재생비율(헤르츠)
	const int m_iBufferCnt;						// 버퍼 카운트

private:
	Vector2 m_vRenderResolution;	// 렌더링 해상도
	HWND m_hWnd;					// 출력 윈도우
	bool m_bWindowMode;				// 출력 모드 (창모드)

private:
	// D3D11 Version
	ComPtr<ID3D11Device> m_pDevice;		// 장치 인터페이스(객체 생성, 메모리 관리)
	ComPtr<ID3D11DeviceContext> m_pContext;// 장치 인터페이스(렌더링)

	ComPtr<IDXGISwapChain> m_pSwapChain;
	ComPtr<ID3D11Texture2D> m_pRTTex;		// RTTex : Render Target Texture
	ComPtr<ID3D11RenderTargetView> m_pRTV; // RTV : Render Target View

	ComPtr<ID3D11DepthStencilView> m_pDSV; // DSV : Depth Stencil View
	ComPtr<ID3D11Texture2D> m_pDSTex;		// DSTex : Depth Stencil Texture

	ComPtr<ID3D11SamplerState> m_pSample;
	D3D11_VIEWPORT m_tViewPort;

private:
	const Vector2& _GetRenderResolution() const { return m_vRenderResolution; }

	// OptionFunction
	const DXGI_RATIONAL& _GetDefaultRefreshRate() const { return m_tDefaultRefreshRate; }
	int _GetBufferCnt() const { return m_iBufferCnt; }

public:
	// _bWindowed : 창모드
	int Init(HWND _hOutputWnd, const Vector2& _vRenderResolution, bool _bWindowMode);

	int CreateSwapChain();
	int CreateView();
	void CreateViewport();
	void ClearTarget();
	void Present();
}; 