#include "pch.h"
#include "CCollider2D.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CTransform.h"

CCollider2D::CCollider2D() :
	CCollider(E_ComponentType::Collider2D),
	m_vOffsetPosition{},
	m_vOffsetScale{},
	m_matWorld{}
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::Render()
{
	if (nullptr == m_pMesh || nullptr == m_pMaterial)
		return;

	// �浹ü ��ġ ���� ������Ʈ
	UpdateData();

	// �޽� ����
	m_pMesh->UpdateData();

	// ���� ����
	m_pMaterial->UpdateData();

	// ������
	m_pMesh->Render();
}

void CCollider2D::FinalUpdate()
{
	// �浹ü ũ�� * �浹ü �̵�(��ü ũ��� �̸� ����) *  (��ü ũ�� * ��ü ȸ�� * ��ü �̵�) == �浹ü�� World Matrix
	//                                                  (           World Matrix       )

	Vector2 vOffsetPosition = m_vOffsetPosition / Transform()->GetScale();
	Matrix matTrans = XMMatrixTranslation(vOffsetPosition.x, vOffsetPosition.y, 0.f);
	Matrix matScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, 1.f);

	m_matWorld = matScale * matTrans * Transform()->GetWorldMatrix4x4();
}

void CCollider2D::UpdateData()
{
	static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Transform);
	g_transform.matWorld = m_matWorld;
	pCB->SetData(&g_transform);
	pCB->UpdateData(E_ShaderStage::Vertex);
}