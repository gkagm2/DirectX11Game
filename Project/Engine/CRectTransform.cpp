#include "pch.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CRectTransform.h"

#include "CGameObject.h"

CRectTransform::CRectTransform() :
	CTransform(E_ComponentType::RectTransform),
	m_fWidth(100.f),
	m_fHeight(100.f)
{
}

CRectTransform::~CRectTransform()
{
}

void CRectTransform::FinalUpdate()
{
	CTransform::FinalUpdate();
}

void CRectTransform::UpdateData()
{
	static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::RectTransform);
	CTransform::UpdateData();
	Matrix matScale = XMMatrixScalingFromVector(Vector3{ m_fWidth, m_fHeight, 1.f});
	Matrix matWorld = matScale * GetWOrldMatrix_NoParentScale();

	Matrix matRotX = XMMatrixRotationX(m_vLocalRotation.x);
	Matrix matRotY = XMMatrixRotationY(m_vLocalRotation.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vLocalRotation.z);
	Matrix matRot = matRotX * matRotY * matRotZ;

	Matrix matTrans = XMMatrixTranslation(m_vLocalPosition.x, m_vLocalPosition.y, m_vLocalPosition.z);

	matWorld = matWorld * matRot * matTrans;


	g_rectTransform.matWorld = matWorld;
	g_rectTransform.matView = g_transform.matView;
	g_rectTransform.matProjection = g_transform.matProjection;


	g_rectTransform.fWidth = m_fWidth;
	g_rectTransform.fHeight = m_fHeight;

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