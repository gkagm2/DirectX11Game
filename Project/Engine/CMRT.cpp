#include "pch.h"
#include "CMRT.h"
#include "CDevice.h"

CMRT::CMRT(SharedPtr<CTexture>* _pArrRTTex, Vector4* _pArrClearColor, UINT _iRTCount, SharedPtr<CTexture> _pDSTex, bool _bUseDepthPrev) :
	m_arrRTV{},
	m_pDSTex{_pDSTex},
	m_arrClearColor{},
	m_iRTCount{ _iRTCount },
	m_bUsePrevDS{_bUseDepthPrev},
	m_tViewPort{}

{
	assert(!(m_iRTCount > MAX_RENDER_TARGET_TEX_CNT));
	//memcpy(m_arrRTTex, _pArrRTTex, sizeof(SharedPtr<CTexture>)* m_iRTCount);
	for (size_t i = 0; i < MAX_RENDER_TARGET_TEX_CNT; ++i) {
		m_arrRTTex[i] = _pArrRTTex[i];
	}

	for (size_t i = 0; i < MAX_RENDER_TARGET_TEX_CNT; ++i) {
		m_arrClearColor[i] = _pArrClearColor[i];
	}
	//memcpy(m_arrClearColor, _pArrClearColor, sizeof(Vector4)* m_iRTCount);

	for (size_t i = 0; i < m_iRTCount; ++i)
		m_arrRTV[i] = m_arrRTTex[i]->GetRTV().Get();

	m_tViewPort.TopLeftX = 0;
	m_tViewPort.TopLeftY = 0;
	m_tViewPort.MinDepth = 0;
	m_tViewPort.MaxDepth = 1;
	m_tViewPort.Width = _pArrRTTex[0]->GetWidth();
	m_tViewPort.Height = _pArrRTTex[0]->GetHeight();
}

CMRT::~CMRT()
{

}

void CMRT::UpdateData()
{
	CONTEXT->RSSetViewports(1, &m_tViewPort);

	// ???? Ÿ?? ?? ???? ???ٽ? ???? ???? (OM Set)
	ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
	if (nullptr != m_pDSTex)
		pDSV = m_pDSTex->GetDSV().Get();
	else if(m_bUsePrevDS)
		CONTEXT->OMGetRenderTargets(0, nullptr, pDSV.GetAddressOf());

	CONTEXT->OMSetRenderTargets(MAX_RENDER_TARGET_TEX_CNT, m_arrRTV, pDSV.Get());
}

void CMRT::Clear()
{
	for (size_t i = 0; i < m_iRTCount; ++i)
		CONTEXT->ClearRenderTargetView(m_arrRTTex[i]->GetRTV().Get(), m_arrClearColor[i]);
	if (nullptr != m_pDSTex) {
		CONTEXT->ClearDepthStencilView(m_pDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}
}