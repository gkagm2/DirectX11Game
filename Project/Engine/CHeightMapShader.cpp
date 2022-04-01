#include "pch.h"
#include "CHeightMapShader.h"
#include "CStructuredBuffer.h"

CHeightMapShader::CHeightMapShader() :
	CComputeShader(32, 32, 1),
	m_vScale(0.1f,0.1f),
	m_iBrushIdx(0),
	m_pInputBuffer(nullptr)
{
}

CHeightMapShader::~CHeightMapShader()
{
}


void CHeightMapShader::Excute()
{
	if (nullptr == m_pHeightMapTex)
		return;

	UINT iGroupX = ((UINT)m_pHeightMapTex->GetWidth() / GetGroupPerThreadX() + 1);
	UINT iGroupY = ((UINT)m_pHeightMapTex->GetHeight() / GetGroupPerThreadY() + 1);

	Dispatch(iGroupX, iGroupY, 1);
}

void CHeightMapShader::UpdateData()
{
	// Height map Texture
	if (nullptr != m_pHeightMapTex) {
		m_tInfo.iArr[0] = (int)m_pHeightMapTex->GetWidth();
		m_tInfo.iArr[1] = (int)m_pHeightMapTex->GetHeight();
		m_tInfo.iArr[2] = m_iBrushIdx;
		m_tInfo.v2Arr[0] = m_vScale;
		
		m_pHeightMapTex->UpdateRWData(0);
	}

	// Brush Textrue
	if (nullptr != m_pBrushTex)
		m_pBrushTex->UpdateData(E_ShaderStage::Compute, 0);

	// Picking info
	static UINT iRegisterNum = 16;
	m_pInputBuffer->UpdateData(iRegisterNum, E_ShaderStage::Compute);
}

void CHeightMapShader::Clear()
{
	if (nullptr != m_pHeightMapTex) {
		m_pHeightMapTex->ClearRW(0);
		m_pHeightMapTex = nullptr;
	}
	if (nullptr != m_pBrushTex) {
		m_pBrushTex->Clear(0);
		m_pBrushTex = nullptr;
	}
	if (nullptr != m_pInputBuffer) {
		m_pInputBuffer->Clear();
		m_pInputBuffer->ClearRW();
		m_pInputBuffer = nullptr;
	}
}