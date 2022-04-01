#include "pch.h"
#include "CRaycastShader.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CCamera.h"
#include "CRenderManager.h"
#include "CTransform.h"
#include "CFontManager.h"

CRaycastShader::CRaycastShader() :
	CComputeShader(32, 32, 1),
	m_iXFace{},
	m_iZFace{},
	m_tRay{},
	m_pOutputBuffer{ nullptr }
{
}

CRaycastShader::~CRaycastShader()
{
}

void CRaycastShader::Excute()
{
	UINT iGroupX = ((m_iXFace * 2) / GetGroupPerThreadX()) + 1;
	UINT iGroupY = (m_iZFace / GetGroupPerThreadY()) + 1;

	Dispatch(iGroupX, iGroupY, 1);
}

void CRaycastShader::UpdateData()
{
	m_tInfo.iArr[0] = (int)m_iXFace;
	m_tInfo.iArr[1] = (int)m_iZFace;

	m_tInfo.v4Arr[0] = m_tRay.vStartPos;
	m_tInfo.v4Arr[1] = m_tRay.vDir;

	// Height Map
	if(nullptr != m_pHeightMap)
		m_pHeightMap->UpdateData(E_ShaderStage::Compute, 0);

	// 출력 버퍼	
	m_pOutputBuffer->UpdateDataCS(0); // u0 register
}

void CRaycastShader::Clear()
{
	m_pHeightMap->Clear(0);
	m_pOutputBuffer->Clear();
	m_pOutputBuffer->ClearRW();
}