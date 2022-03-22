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


	CCamera* pCam = CRenderManager::GetInstance()->GetMainCamera();
	if (pCam) {
		Vector3 pos = pCam->Transform()->GetPosition();
		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"pos (%.2f, %.2f. %.2f)", pos.x, pos.y, pos.z);
		CFontManager::GetInstance()->DrawUIFont(szBuffer, 10.f, 90.f, 20, FONT_RGBA(255, 20, 20, 127), FW1_TEXT_FLAG::FW1_LEFT);

		swprintf_s(szBuffer, L"start pos (%.2f, %.2f, %.2f)", m_tRay.vStartPos.x, m_tRay.vStartPos.y, m_tRay.vStartPos.z);
		CFontManager::GetInstance()->DrawUIFont(szBuffer, 10.f, 120.f, 20, FONT_RGBA(255, 20, 20, 127), FW1_TEXT_FLAG::FW1_LEFT);
	}


	// Height Map
	if (nullptr != m_pHeightMap) {
		m_pHeightMap->UpdateData(E_ShaderStage::Compute, 0);
	}

	// 출력 버퍼	
	m_pOutputBuffer->UpdateDataCS(0); // u0 register
}

void CRaycastShader::Clear()
{
	m_pHeightMap->Clear(0);
	m_pOutputBuffer->ClearRW();
}