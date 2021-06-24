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
	// ũ�� ���
	Matrix matScale = XMMatrixScaling(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);

	// ȸ�� ���
	Matrix matRotX = XMMatrixRotationX(m_vLocalRotation.x);
	Matrix matRotY = XMMatrixRotationY(m_vLocalRotation.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vLocalRotation.z);

	Matrix matRot = matRotX * matRotY * matRotZ;

	// �̵� ���
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
	// b0 �������Ϳ� ������� ���ε�
	static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Transform);

	g_transform.matWorld = m_matWorld;

	pCB->SetData(&g_transform);
	pCB->UpdateData(E_ShaderStage::Vertex);
}
