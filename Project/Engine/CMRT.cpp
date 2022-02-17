#include "pch.h"
#include "CMRT.h"
#include "CDevice.h"

CMRT::CMRT(SharedPtr<CTexture> _pArrRTTex[MAX_RENDER_TARGET_TEX_CNT], Vector4 _pArrClearColor[MAX_RENDER_TARGET_TEX_CNT], UINT _iRTCount, SharedPtr<CTexture> _pDSTex, bool _bUseDepthPrev) :
	m_arrRTV{},
	m_pDSTex{_pDSTex},
	m_arrClearColor{},
	m_iRTCount{ _iRTCount },
	m_bUsePrevDS{_bUseDepthPrev}
{
	assert(!(m_iRTCount > MAX_RENDER_TARGET_TEX_CNT));
	memcpy(m_arrRTTex, _pArrRTTex, sizeof(SharedPtr<CTexture>)* m_iRTCount);
	memcpy(m_arrClearColor, _pArrClearColor, sizeof(Vector4)* m_iRTCount);

	for (size_t i = 0; i < m_iRTCount; ++i)
		m_arrRTV[i] = m_arrRTTex[i]->GetRTV().Get();
}

CMRT::~CMRT()
{
}

void CMRT::UpdateData()
{
	// Ãâ·Â Å¸°Ù ¹× ±íÀÌ ½ºÅÙ½Ç ¹öÆÛ ¼³Á¤ (OM Set)
	ID3D11DepthStencilView* pDSV = nullptr;
	if (nullptr != m_pDSTex)
		pDSV = m_pDSTex->GetDSV().Get();
	CONTEXT->OMSetRenderTargets(m_iRTCount, m_arrRTV, pDSV);
}

void CMRT::Clear()
{
	for (size_t i = 0; i < m_iRTCount; ++i)
		CONTEXT->ClearRenderTargetView(m_arrRTTex[i]->GetRTV().Get(), m_arrClearColor[i]);
	if (nullptr != m_pDSTex) {
		CONTEXT->ClearDepthStencilView(m_pDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}
}