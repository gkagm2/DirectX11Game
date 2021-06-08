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
	CConstBuffer* m_arrCB[(UINT)E_ConstBuffer::End];
	ComPtr<ID3D11SamplerState> m_pSamplerStates[2];
	ComPtr<ID3D11RasterizerState> m_pRasterizerStates[(UINT)E_RasterizerState::End];
	ComPtr<ID3D11BlendState> m_pBlendStates[(UINT)E_BlendState::End];

	// D3D11 Version
	ComPtr<ID3D11Device> m_pDevice;			// ��ġ �������̽�(��ü ����, ����, �޸� ����)
	ComPtr<ID3D11DeviceContext> m_pContext; // ��ġ �������̽�(������, ���� �ٲٱ�) ���� ������ ���ؽ�Ʈ

	ComPtr<IDXGISwapChain> m_pSwapChain;
	ComPtr<ID3D11Texture2D> m_pRTTex;		// RTTex : Render Target Texture (Back buffer)
	ComPtr<ID3D11RenderTargetView> m_pRTV;	// RTV : Render Target View

	ComPtr<ID3D11DepthStencilView> m_pDSV;	// DSV : Depth Stencil View
	ComPtr<ID3D11Texture2D> m_pDSTex;		// DSTex : Depth Stencil Texture
	
	D3D11_VIEWPORT m_tViewPort;

private:
	// OptionFunction
	const DXGI_RATIONAL& _GetDefaultRefreshRate() const { return m_tDefaultRefreshRate; }
	int _GetBufferCnt() const { return m_iBufferCnt; }

public:
	const Vector2& GetRenderResolution() const { return m_vRenderResolution; }
	float GetAspectRatio() { return m_vRenderResolution.x == 0.0f ? 0.f : m_vRenderResolution.x / m_vRenderResolution.y; }

	void SetRasterizerState(E_RasterizerState _eRasterizerState) { CONTEXT->RSSetState(m_pRasterizerStates[(UINT)_eRasterizerState].Get()); }

	ComPtr<ID3D11BlendState> GetBlendState(E_BlendState _eBlendState) { return m_pBlendStates[(UINT)_eBlendState]; }


public:
	// _bWindowed : â���
	int Init(HWND _hOutputWnd, const Vector2& _vRenderResolution, bool _bWindowMode);

	void ClearTarget();
	void Present();

	ComPtr<ID3D11Device> GetDevice() { return m_pDevice; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return m_pContext; }
	const CConstBuffer* GetConstBuffer(E_ConstBuffer _eType) { return m_arrCB[(UINT)_eType]; }

private:
	int CreateSwapChain(); 
	int CreateView();
	void CreateViewport();
	void CreateConstBuffer();
	void CreateSampler();
	void CreateRasterizerState();

	// OM stage
	void CreateBlendState();
	void CreateDepthStencilState();
};