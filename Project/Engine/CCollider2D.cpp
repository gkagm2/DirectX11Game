#include "pch.h"
#include "CResourceManager.h"
#include "CCollider2D.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CTransform.h"
#include "CScript.h"
#include "CConfigurationManager.h"
#include "CCollisionManager.h"
#include "CSceneManager.h"
#include "CScene.h"

CCollider2D::CCollider2D() :
	CCollider(E_ComponentType::Collider2D),
	m_vOffsetPosition{},
	m_vOffsetScale{Vector2(1.f,1.f)},
	m_matColWorld{},
	m_pMesh(nullptr),
	m_pMaterial(nullptr)
{
	m_pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectLineMesh);

	m_pMaterial = m_pNonColliedMaterial;

	SetMesh(m_pMesh);
	SetMaterial(m_pMaterial);
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

	// 재질 세팅
	m_pMaterial->UpdateData();

	// 렌더링
	m_pMesh->Render();

	// 메터리얼 클리어
	m_pMaterial->Clear();
}

void CCollider2D::FinalUpdate()
{
	// 충돌체 크기 * 충돌체 이동(물체 크기로 미리 나눔) *  (물체 크기 * 물체 회전 * 물체 이동) == 충돌체의 World Matrix
	//                                                  (           World Matrix       )
	Vector2 vOffsetPosition = m_vOffsetPosition  / Transform()->GetScale().XY();

	//vOffsetPosition *= fUnit;
	
	Matrix matTrans = XMMatrixTranslation(vOffsetPosition.x, vOffsetPosition.y, 0.f);
	Matrix matScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, 1.f);

	m_matColWorld = matScale * matTrans * Transform()->GetWorldMatrix();
	m_matColWorld._43 = 0.f; // 2D이므로 0
}

void CCollider2D::UpdateData()
{
	static const CConstBuffer* pCB = CDevice::GetInstance()->GetConstBuffer(E_ConstBuffer::Transform);
	g_transform.matWorld = m_matColWorld;
	g_transform.matWorldView = g_transform.matWorld * g_transform.matView;
	g_transform.matWorldViewProj = g_transform.matWorldView * g_transform.matProjection;

	pCB->SetData(&g_transform);
	pCB->UpdateData(E_ShaderStage::Vertex);
}

void CCollider2D::OnCollisionEnter2D(CCollider2D* _pOther)
{
	IncreaseCollisionCnt();
	m_pMaterial = m_pCollisionMaterial;

	vector<CScript*>& vecScripts = GetGameObject()->_GetScripts();
	for (UINT i = 0; i < vecScripts.size(); ++i)
		vecScripts[i]->OnCollisionEnter2D(_pOther);
}

void CCollider2D::OnCollisionStay2D(CCollider2D* _pOther)
{
	vector<CScript*>& vecScripts = GetGameObject()->_GetScripts();
	for (UINT i = 0; i < vecScripts.size(); ++i)
		vecScripts[i]->OnCollisionStay2D(_pOther);
}

void CCollider2D::OnCollisionExit2D(CCollider2D* _pOther)
{
	DecreaseCollisionCnt();
	if (0 == m_iCollisionCount)
		m_pMaterial = m_pNonColliedMaterial;

	vector<CScript*>& vecScripts = GetGameObject()->_GetScripts();
	for (UINT i = 0; i < vecScripts.size(); ++i)
		vecScripts[i]->OnCollisionExit2D(_pOther);
}

bool CCollider2D::SaveToScene(FILE* _pFile)
{
	CCollider::SaveToScene(_pFile);

	FWrite(m_vOffsetPosition, _pFile);
	FWrite(m_vOffsetScale, _pFile);

	SaveResourceToFile(m_pMesh, _pFile);
	SaveResourceToFile(m_pMaterial, _pFile);

	return true;
}

bool CCollider2D::LoadFromScene(FILE* _pFile)
{
	CCollider::LoadFromScene(_pFile);
	FRead(m_vOffsetPosition, _pFile);
	FRead(m_vOffsetScale, _pFile);

	LoadResourceFromFile(m_pMesh, _pFile);
	LoadResourceFromFile(m_pMaterial, _pFile);
	return true;
}