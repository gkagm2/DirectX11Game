#include "pch.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CRectTransform.h"
#include "CRenderManager.h"
#include "CCamera.h"

#include "CGameObject.h"

CRectTransform::CRectTransform() :
	CTransform(E_ComponentType::RectTransform),
	m_fWidth(100.f),
	m_fHeight(100.f),
	m_matLocal_noParentScale{},
	m_matWorld_noParentScale{}
{
}

CRectTransform::~CRectTransform()
{
}

void CRectTransform::FinalUpdate()
{
	{
		//CTransform::FinalUpdate();
		// 크기 행렬
		Matrix matScale = XMMatrixScaling(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);

		// 회전 행렬
		Matrix matRotX = XMMatrixRotationX(m_vLocalRotation.x);
		Matrix matRotY = XMMatrixRotationY(m_vLocalRotation.y);
		Matrix matRotZ = XMMatrixRotationZ(m_vLocalRotation.z);

		Matrix matRot = matRotX * matRotY * matRotZ;
		// 이동 행렬
		Matrix matTrans = XMMatrixTranslation(m_vLocalPosition.x, m_vLocalPosition.y, m_vLocalPosition.z);

		m_matWorld = m_matLocal = matScale * matRot * matTrans;

		CGameObject* pParentObj = GetGameObject()->GetParentObject();
		if (pParentObj) {
			if (pParentObj->RectTransform()) {
				const Matrix& matParentWorld = pParentObj->RectTransform()->GetWorldMatrix();
				m_matWorld *= matParentWorld;
			}
			else {
				const Matrix& matParentWorld = pParentObj->Transform()->GetWorldMatrix();
				m_matWorld *= matParentWorld;
			}
		}

		float fSize = 1.f;
		if (CRenderManager::GetInstance()->GetMainCamera()) {
			CCamera* pMainCam = CRenderManager::GetInstance()->GetMainCamera();
			if (E_ProjectionType::Orthographic == pMainCam->GetProjectionType()) {
				fSize = pMainCam->GetSize();
			}
		}

		Matrix matScaleWH = XMMatrixScaling(m_vLocalScale.x * m_fWidth, m_vLocalScale.y * m_fHeight, m_vLocalScale.z);

		m_matLocal_noParentScale = matScaleWH * matRot * matTrans * fSize;
		m_matWorld_noParentScale = m_matLocal_noParentScale;
		pParentObj = GetGameObject()->GetParentObject();
		if (pParentObj) {
			if (pParentObj->RectTransform()) {
				const Matrix& matParentWorld = pParentObj->RectTransform()->GetWorldMatrix();
				m_matWorld_noParentScale *= matParentWorld;
			}
			else {
				const Matrix& matParentWorld = pParentObj->Transform()->GetWorldMatrix();
				m_matWorld_noParentScale *= matParentWorld;
			}
		}
	}
}

void CRectTransform::UpdateData()
{
	static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::RectTransform);
	CTransform::UpdateData();

	g_rectTransform.matWorld = m_matWorld_noParentScale;
	g_rectTransform.matView = g_transform.matView;
	g_rectTransform.matProjection = g_transform.matProjection;

	pCB->SetData(&g_rectTransform);
	pCB->UpdateData(E_ShaderStage::NonePixel);
}

bool CRectTransform::SaveToScene(FILE* _pFile)
{
	CTransform::SaveToScene(_pFile);

	FWrite(m_fWidth, _pFile);
	FWrite(m_fHeight, _pFile);
	return true;
}

bool CRectTransform::LoadFromScene(FILE* _pFile)
{
	CTransform::LoadFromScene(_pFile);
	
	FRead(m_fWidth, _pFile);
	FRead(m_fHeight, _pFile);
	return true;
}