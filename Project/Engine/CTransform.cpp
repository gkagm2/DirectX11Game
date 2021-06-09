#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"
#include "CDevice.h"

// Test
#include "CKeyManager.h"
#include "CTimeManager.h"


CTransform::CTransform() :
	CComponent(E_ComponentType::Transform),
	m_vLocalPosition{},
	m_vLocalScale{1.f, 1.f, 1.f},
	m_vLocalRotation{},
	m_matWorld{}
{
}

CTransform::~CTransform()
{
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

	m_matWorld = matScale * matRot * matTrans;
}

void CTransform::UpdateData()
{
	// b0 �������Ϳ� ������� ���ε�
	static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Transform);

	g_transform.matWorld = m_matWorld;

	pCB->SetData(&g_transform);
	pCB->UpdateData(E_ShaderStage::Vertex);
}
