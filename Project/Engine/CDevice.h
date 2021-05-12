#pragma once
#include "CSingleton.h"
class CDevice : public CSingleton<CDevice>
{
	SINGLETON(CDevice)

private:
	HWND m_hWnd;		// 출력 윈도우
	bool m_bWindowMode; // 출력 모드

private:
	// D3D11 Version
	ID3D11Device* m_pDevice;		// 장치 인터페이스(객체 생성, 메모리 관리)
	ID3D11DeviceContext* m_pContext; // 장치 인터페이스(렌더링)

	IDXGISwapChain* m_pSwapChain;
	ID3D11Texture2D* m_pRTTex; // RTTex : Render Target Texture
	ID3D11RenderTargetView* m_pRTV; // RTV : Render Target View

	ID3D11DepthStencilView* m_pDSV; // DSV : Depth Stencil View
	ID3D11Texture2D* m_pDSTex; // DSTex : Depth Stencil Texture

	ID3D11SamplerState* m_pSample;


public:
	// _bWindowed : 창모드
	int Init(HWND _hOutputWnd, bool _bWindowMode);
}; 