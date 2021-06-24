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
	// Render
	// 1. Ÿ�� Ŭ����
	CDevice::GetInstance()->ClearTarget();

	// 2. ī�޶� �������� ������
	for (UINT i = 0; i < m_vecCam.size(); ++i)
		m_vecCam[i]->Render();

	// 3. ����� ��ü
	CDevice::GetInstance()->Present();

	m_vecCam.clear();
	m_vecLight2D.clear();
}

void CRenderManager::_RenderInit()
{
	// ���ҽ� ���ε�
	struct {
		TLightInfo arrLight2D[100] = {}; // TODO (Jang) : �ӽ�
		int iCount = 0;
		int iPadding[3] = {};
	}tInfo;
	
	for (UINT i = 0; i < m_vecLight2D.size(); ++i)
		tInfo.arrLight2D[i] = m_vecLight2D[i]->GetLightInfo();
	tInfo.iCount = (int)m_vecLight2D.size();

	// ����Ʈ ���ۿ� �÷��� GPU�� ����.

	static const CConstBuffer* pLight2DBuffer = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Light2D);
	pLight2DBuffer->SetData(&tInfo);
	pLight2DBuffer->UpdateData();
}