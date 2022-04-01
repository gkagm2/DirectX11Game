#include "pch.h"
#include "CWeightMapShader.h"
#include "CStructuredBuffer.h"

CWeightMapShader::CWeightMapShader() :
	CComputeShader(32, 32, 1),
	m_pWeightMapBuff{ nullptr },
	m_pRaycastInfoBuff{ nullptr },
	m_vBrushScale{ 1.f,1.f },
	m_iBrushIdx{ 0 },
	m_iWeightIdx{ 0 },
	m_iWidth{ 0 },
	m_iHeight{ 0 }
{
}

CWeightMapShader::~CWeightMapShader()
{
}

void CWeightMapShader::UpdateData()
{
	m_tInfo.iArr[0] = m_iWidth;
	m_tInfo.iArr[1] = m_iHeight;
	m_tInfo.iArr[2] = m_iBrushIdx;
	m_tInfo.iArr[3] = m_iWeightIdx;
	m_tInfo.v2Arr[0] = m_vBrushScale;

	m_pWeightMapBuff->UpdateDataCS(0);
	m_pBrushArrTex->UpdateData(E_ShaderStage::Compute, 0);
	m_pRaycastInfoBuff->UpdateData(16, E_ShaderStage::Compute);
}

void CWeightMapShader::Excute()
{
	if (nullptr == m_pWeightMapBuff)
		return;

	UINT iGroupX = (m_iWidth / GetGroupPerThreadX()) + 1;
	UINT iGroupY = (m_iHeight / GetGroupPerThreadY()) + 1;

	Dispatch(iGroupX, iGroupY, 1);
}

void CWeightMapShader::Clear()
{
	if (nullptr != m_pWeightMapBuff) {
		m_pWeightMapBuff->Clear();
		m_pWeightMapBuff->ClearRW();
		m_pWeightMapBuff = nullptr;
	}

	if (nullptr != m_pBrushArrTex) {
		m_pBrushArrTex->Clear(0);
		m_pBrushArrTex = nullptr;
	}

	if (nullptr != m_pRaycastInfoBuff) {
		m_pRaycastInfoBuff->Clear();
		m_pRaycastInfoBuff->ClearRW();
		m_pRaycastInfoBuff = nullptr;
	}
}