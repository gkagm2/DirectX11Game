#include "pch.h"
#include "CResourceManager.h"
#include "CCollider2D.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CTransform.h"

CCollider2D::CCollider2D() :
	CCollider(E_ComponentType::Collider2D),
	m_vOffsetPosition{},
	m_vOffsetScale{Vector2(1.f,1.f)},
	m_matColWorld{}
{
	m_pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMash);
	m_pMaterial = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_Collider2DMaterial);
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

	Vector2 vOffsetPosition = m_vOffsetPosition / Transform()->GetScale().XY();
	Matrix matTrans = XMMatrixTranslation(vOffsetPosition.x, vOffsetPosition.y, 0.f);
	Matrix matScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, 1.f);

	m_matColWorld = matScale * matTrans * Transform()->GetWorldMatrix();
}

void CCollider2D::UpdateData()
{
	static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Transform);
	g_transform.matWorld = m_matColWorld;
	pCB->SetData(&g_transform);
	pCB->UpdateData(E_ShaderStage::Vertex);
}