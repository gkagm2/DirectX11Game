#include "pch.h"
#include "CRenderManager.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CSceneManager.h"

CRenderManager::CRenderManager()
{

}

CRenderManager::~CRenderManager()
{

}
void CRenderManager::Init()
{
}

void CRenderManager::Render()
{
	_RenderInit();

	// Render
	// 1. 타겟 클리어
	CDevice::GetInstance()->ClearTarget();

	// 2. 카메라 기준으로 렌더링
	for (UINT i = 0; i < m_vecCam.size(); ++i)
		m_vecCam[i]->Render();

	// 3. 백버퍼 교체
	CDevice::GetInstance()->Present();

	m_vecCam.clear();
	m_vecLight2D.clear();
}

void CRenderManager::_RenderInit()
{
	// 리소스 바인딩
	struct {
		TLightInfo arrLight2D[100] = {}; // TODO (Jang) : 임시
		int iCount = 0;
		int iPadding[3] = {};
	}tInfo;
	
	for (UINT i = 0; i < m_vecLight2D.size(); ++i)
		tInfo.arrLight2D[i] = m_vecLight2D[i]->GetLightInfo();
	tInfo.iCount = (int)m_vecLight2D.size();

	// 컨스트 버퍼에 올려서 GPU에 넣자.

	static const CConstBuffer* pLight2DBuffer = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Light2D);
	pLight2DBuffer->SetData(&tInfo);
	pLight2DBuffer->UpdateData();
}