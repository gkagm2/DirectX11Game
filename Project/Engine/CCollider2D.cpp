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

	// 충돌체 위치 정보 업데이트
	UpdateData();

	// 메쉬 세팅
	m_pMesh->UpdateData();

	// 재질 세팅
	m_pMaterial->UpdateData();

	// 렌더링
	m_pMesh->Render();
}

void CCollider2D::FinalUpdate()
{
	// 충돌체 크기 * 충돌체 이동(물체 크기로 미리 나눔) *  (물체 크기 * 물체 회전 * 물체 이동) == 충돌체의 World Matrix
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