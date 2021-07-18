#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"
#include "CDevice.h"

#include "CGameObject.h"

// Test
CTransform::CTransform() :
	CComponent(E_ComponentType::Transform),
	m_vLocalPosition{},
	m_vLocalScale{1.f, 1.f, 1.f},
	m_vLocalRotation{},
	m_matLocal{},
	m_matWorld{}
{
}

CTransform::~CTransform()
{
}

Vector3 CTransform::GetScale()
{
	Vector3 vWorldScale = m_vLocalScale;
	CGameObject* pGameObj = GetGameObject();

	while (pGameObj->GetParentObject()) {
		vWorldScale *= pGameObj->GetParentObject()->Transform()->GetLocalScale();
		pGameObj = pGameObj->GetParentObject();
	}
	return vWorldScale;
}

void CTransform::FinalUpdate()
{
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
		const Matrix& matParentWorld = pParentObj->Transform()->GetWorldMatrix();
		m_matWorld *= matParentWorld;
	}
}

void CTransform::UpdateData()
{
	// b0 레지스터에 상수버퍼 바인딩
	static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Transform);

	g_transform.matWorld = m_matWorld;
	g_transform.matWorldView = g_transform.matWorld * g_transform.matView;
	g_transform.matWorldViewProj = g_transform.matWorldView * g_transform.matProjection;

	pCB->SetData(&g_transform);
	pCB->UpdateData(E_ShaderStage::NonePixel);
}

bool CTransform::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);
	FWrite(m_vLocalPosition, _pFile);
	FWrite(m_vLocalRotation, _pFile);
	FWrite(m_vLocalScale, _pFile);
	return true;
}

bool CTransform::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);
	FRead(m_vLocalPosition, _pFile);
	FRead(m_vLocalRotation, _pFile);
	FRead(m_vLocalScale, _pFile);
	return true;
}