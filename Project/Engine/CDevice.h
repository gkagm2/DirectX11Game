#pragma once
#include "CSingleton.h"
#include "CTexture.h"

class CConstBuffer;
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
	CConstBuffer* m_arrCB[(UINT)E_ConstBuffer::End];
	ComPtr<ID3D11SamplerState> m_pSamplerStates[2];
	ComPtr<ID3D11RasterizerState> m_pRasterizerStates[(UINT)E_RasterizerState::End];
	ComPtr<ID3D11BlendState> m_pBlendStates[(UINT)E_BlendState::End];
	ComPtr<ID3D11DepthStencilState> m_pDepthStencilStates[(UINT)E_DepthStencilState::End];

	// D3D11 Version
	ComPtr<ID3D11Device> m_pDevice;			// 장치 인터페이스(객체 생성, 해제, 메모리 관리)
	ComPtr<ID3D11DeviceContext> m_pContext; // 장치 인터페이스(렌더링, 상태 바꾸기) 현재 상태의 컨텍스트

	ComPtr<IDXGISwapChain> m_pSwapChain;
	ComPtr<ID3D11Texture2D> m_pRTTex;		// RTTex : Render Target Texture (Back buffer)
	ComPtr<ID3D11RenderTargetView> m_pRTV;	// RTV : Render Target View

	SharedPtr<CTexture> m_pDSTex;		// DSTex : Depth Stencil Texture
	
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

	void SetBlendState(E_BlendState _eBlendState, Vector4 _vBlendFactor = Vector4(0.f, 0.f, 0.f, 0.f), UINT _SampleMask = 0xffffffff) {
		CONTEXT->OMSetBlendState(m_pBlendStates[(UINT)_eBlendState].Get(), _vBlendFactor, _SampleMask);
	}

	void SetDepthStencilState(E_DepthStencilState _eState, UINT _iStencilRef = 0) { CONTEXT->OMSetDepthStencilState(m_pDepthStencilStates[(UINT)_eState].Get(), _iStencilRef); }
public:
	// _bWindowed : 창모드
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