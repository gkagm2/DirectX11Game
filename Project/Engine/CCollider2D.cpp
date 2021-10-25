#include "pch.h"
#include "CResourceManager.h"
#include "CCollider2D.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CTransform.h"
#include "CScript.h"
#include "CConfigurationManager.h"

queue<CMaterial*> g_queCollisionMtrl; // �浹 �� ������ ���͸����� ���� ��

CCollider2D::CCollider2D() :
	CCollider(E_ComponentType::Collider2D),
	m_vOffsetPosition{},
	m_vOffsetScale{Vector2(1.f,1.f)},
	m_matColWorld{},
	m_pMesh(nullptr),
	m_pMaterial(nullptr)
{
	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->LoadRes<CMesh>(STR_KEY_RectLineMesh);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_Collider2DMtrl);
	SetMesh(pMesh);
	SetMaterial(pMtrl);
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
	Vector2 vOffsetPosition = m_vOffsetPosition  / Transform()->GetScale().XY();

	//vOffsetPosition *= fUnit;
	
	Matrix matTrans = XMMatrixTranslation(vOffsetPosition.x, vOffsetPosition.y, 0.f);



	Matrix matScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, 1.f);

	m_matColWorld = matScale * matTrans * Transform()->GetWorldMatrix();
	m_matColWorld._43 = 0.f; // 2D�̹Ƿ� 0
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
	// ī��Ʈ�� 0�� ���¸�
	if (m_iCollisionCount == 0) {
		// ���͸����� �ϳ� �����ߵǰ���
		if (g_queCollisionMtrl.empty()) {
			// �浹 ���͸����� ������ ���� �����ؼ� que���ٰ� �ֱ�?
			g_queCollisionMtrl.push(m_pMaterial->Clone());
		}
		m_pMaterial = g_queCollisionMtrl.front();
		g_queCollisionMtrl.pop();

		int iConnectColor = 1;// 1 or 0
		m_pMaterial->SetData(E_ShaderParam::Int_0, &iConnectColor);
	}
	IncreaseCollisionCnt();

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
	if (0 == m_iCollisionCount) {
		g_queCollisionMtrl.push(m_pMaterial.Get());
		m_pMaterial = CResourceManager::GetInstance()->LoadRes<CMaterial>(STR_KEY_Collider2DMtrl);

	}
	vector<CScript*>& vecScripts = GetGameObject()->_GetScripts();
	for (UINT i = 0; i < vecScripts.size(); ++i)
		vecScripts[i]->OnCollisionExit2D(_pOther);
}

bool CCollider2D::SaveToScene(FILE* _pFile)
{
	FWrite(m_vOffsetPosition, _pFile);
	FWrite(m_vOffsetScale, _pFile);

	SaveResourceToFile(m_pMesh, _pFile);
	SaveResourceToFile(m_pMaterial, _pFile);

	return true;
}

bool CCollider2D::LoadFromScene(FILE* _pFile)
{
	FRead(m_vOffsetPosition, _pFile);
	FRead(m_vOffsetScale, _pFile);

	LoadResourceFromFile(m_pMesh, _pFile);
	LoadResourceFromFile(m_pMaterial, _pFile);
	return true;
}