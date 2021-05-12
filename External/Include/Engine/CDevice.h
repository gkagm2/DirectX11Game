#pragma once
#include "CSingleton.h"
class CDevice : public CSingleton<CDevice>
{
	SINGLETON(CDevice)

private:
	HWND m_hWnd;		// ��� ������
	bool m_bWindowMode; // ��� ���

private:
	// D3D11 Version
	ID3D11Device* m_pDevice;		// ��ġ �������̽�(��ü ����, �޸� ����)
	ID3D11DeviceContext* m_pContext; // ��ġ �������̽�(������)

	IDXGISwapChain* m_pSwapChain;
	ID3D11Texture2D* m_pRTTex; // RTTex : Render Target Texture
	ID3D11RenderTargetView* m_pRTV; // RTV : Render Target View

	ID3D11DepthStencilView* m_pDSV; // DSV : Depth Stencil View
	ID3D11Texture2D* m_pDSTex; // DSTex : Depth Stencil Texture

	ID3D11SamplerState* m_pSample;


public:
	// _bWindowed : â���
	int Init(HWND _hOutputWnd, bool _bWindowMode);
}; 