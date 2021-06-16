#include "pch.h"
#include "CResourceManager.h"
#include "CCollider2D.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CTransform.h"

queue<SharedPtr<CMaterial>> g_queCollisionMtrl; // 충돌 시 생성된 메터리얼을 담을 곳

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
	// 카운트가 0인 상태면
	//if (m_iCollisionCount == 0) {
	//	// 메터리얼을 하나 만들어야되겠지
	//	if (g_queCollisionMtrl.empty()) {
	//		// 충돌 메터리얼이 없으면 새로 생성해서 que에다가 넣기?
	//		g_queCollisionMtrl.push(m_pMaterial->Clone());
	//		m_pMaterial = g_queCollisionMtrl.front();
	//		g_queCollisionMtrl.pop();

	//		int iTest = 1;
	//		m_pMaterial->SetData(E_ShaderParam::Int_0, &iTest);
	//	}
	//}
	IncreaseCollisionCnt();
}

void CCollider2D::OnCollisionStay(CCollider2D* _pOther)
{
}

void CCollider2D::OnCollisionExit(CCollider2D* _pOther)
{
	DecreaseCollisionCnt();
	/*if (0 == m_iCollisionCount) {
		g_queCollisionMtrl.push(m_pMaterial);
		m_pMaterial = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_Collider2DMaterial);
	}*/
}