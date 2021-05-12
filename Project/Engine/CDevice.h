#pragma once
#include "CSingleton.h"
class CDevice : public CSingleton<CDevice>
{
	SINGLETON(CDevice)

private:
	// Swap Chain Property
	DXGI_RATIONAL m_tDefaultRefreshRate;		// �⺻ ����� �������(�츣��)
	const int m_iBufferCnt;						// ���� ī��Ʈ

private:
	Vector2 m_vRenderResolution;	// ������ �ػ�
	HWND m_hWnd;					// ��� ������
	bool m_bWindowMode;				// ��� ��� (â���)

private:
	// D3D11 Version
	ID3D11Device* m_pDevice;		// ��ġ �������̽�(��ü ����, �޸� ����)
	ID3D11DeviceContext* m_pContext;// ��ġ �������̽�(������)

	IDXGISwapChain* m_pSwapChain;
	ID3D11Texture2D* m_pRTTex;		// RTTex : Render Target Texture
	ID3D11RenderTargetView* m_pRTV; // RTV : Render Target View

	ID3D11DepthStencilView* m_pDSV; // DSV : Depth Stencil View
	ID3D11Texture2D* m_pDSTex;		// DSTex : Depth Stencil Texture

	ID3D11SamplerState* m_pSample;

private:
	const Vector2& _GetRenderResolution() const { return m_vRenderResolution; }

	// OptionFunction
	const DXGI_RATIONAL& _GetDefaultRefreshRate() const { return m_tDefaultRefreshRate; }
	int _GetBufferCnt() const { return m_iBufferCnt; }

public:
	// _bWindowed : â���
	int Init(HWND _hOutputWnd, const Vector2& _vRenderResolution, bool _bWindowMode);

	int CreateSwapChain();
	int CreateView();
}; 