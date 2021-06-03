#pragma once
#include "CSingleton.h"

class CConstBuffer;
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
	CConstBuffer* m_arrCB[(UINT)E_ConstBuffer::end];

	// D3D11 Version
	ComPtr<ID3D11Device> m_pDevice;			// ��ġ �������̽�(��ü ����, ����, �޸� ����)
	ComPtr<ID3D11DeviceContext> m_pContext; // ��ġ �������̽�(������, ���� �ٲٱ�) ���� ������ ���ؽ�Ʈ

	ComPtr<IDXGISwapChain> m_pSwapChain;
	ComPtr<ID3D11Texture2D> m_pRTTex;		// RTTex : Render Target Texture (Back buffer)
	ComPtr<ID3D11RenderTargetView> m_pRTV;	// RTV : Render Target View

	ComPtr<ID3D11DepthStencilView> m_pDSV;	// DSV : Depth Stencil View
	ComPtr<ID3D11Texture2D> m_pDSTex;		// DSTex : Depth Stencil Texture

	ComPtr<ID3D11SamplerState> m_pSample;
	D3D11_VIEWPORT m_tViewPort;

private:
	// OptionFunction
	const DXGI_RATIONAL& _GetDefaultRefreshRate() const { return m_tDefaultRefreshRate; }
	int _GetBufferCnt() const { return m_iBufferCnt; }

public:
	const Vector2& GetRenderResolution() const { return m_vRenderResolution; }
public:
	// _bWindowed : â���
	int Init(HWND _hOutputWnd, const Vector2& _vRenderResolution, bool _bWindowMode);

	void ClearTarget();
	void Present();

	ComPtr<ID3D11Device> GetDevice() { return m_pDevice; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return m_pContext; }
	const CConstBuffer* GetConstBuffer(E_ConstBuffer _eType) { return m_arrCB[(UINT)_eType]; }

	int CreateSwapChain(); 
	int CreateView();
	void CreateViewport();
	void CreateConstBuffer();
};