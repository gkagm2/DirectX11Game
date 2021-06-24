#include "pch.h"
#include "CResourceManager.h"
#include "CCollider2D.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CTransform.h"

queue<CMaterial*> g_queCollisionMtrl; // �浹 �� ������ ���͸����� ���� ��

CCollider2D::CCollider2D() :
	CCollider(E_ComponentType::Collider2D),
	m_vOffsetPosition{},
	m_vOffsetScale{Vector2(1.f,1.f)},
	m_matColWorld{},
	m_pMesh(nullptr),
	m_pMaterial(nullptr)
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

	// ���͸��� Ŭ����
	m_pMaterial->Clear();
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

void CCollider2D::OnCollisionEnter(CCollider2D* _pOther)
{
	// ī��Ʈ�� 0�� ���¸�
	if (m_iCollisionCount == 0) {
		// ���͸����� �ϳ� �����ߵǰ���
		if (g_queCollisionMtrl.empty()) {
			// �浹 ���͸����� ������ ���� �����ؼ� que���ٰ� �ֱ�?
			g_queCollisionMtrl.push(m_pMaterial->Clone());
		}
		m_pMaterial = g_queCollisionMtrl.front();
		g_queCollisionMtrl.pop();

		int iTest = 1;
		m_pMaterial->SetData(E_ShaderParam::Int_0, &iTest);
	}
	IncreaseCollisionCnt();
}

void CCollider2D::OnCollisionStay(CCollider2D* _pOther)
{
}

void CCollider2D::OnCollisionExit(CCollider2D* _pOther)
{
	DecreaseCollisionCnt();
	if (0 == m_iCollisionCount) {
		g_queCollisionMtrl.push(m_pMaterial.Get());
		m_pMaterial = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_Collider2DMaterial);
	}
}