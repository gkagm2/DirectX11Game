#include "pch.h"
#include "CMesh.h"
#include "CMeshRenderer.h"
#include "CGraphicsShader.h"
#include "CTexture.h"

// TODO (Jang) : Test code
#include "CGameObject.h"
#include "CTransform.h"
#include "CCollider2D.h"

CMeshRenderer::CMeshRenderer() :
	CRenderer(E_ComponentType::MeshRenderer),
	m_pMesh(nullptr),
	m_pMtrl(nullptr)
{
}

CMeshRenderer::~CMeshRenderer()
{
}

void CMeshRenderer::Render()
{
	if(nullptr == m_pMesh || nullptr == m_pMtrl)
		return;

	// TODO (Jang) : Test code
	GetGameObject()->GetComponent<CTransform>()->UpdateData(); // Test code

	m_pMesh->UpdateData();   // 메쉬 세팅
	m_pMtrl->UpdateData();	 // 메터리얼 세팅
	m_pMesh->Render();		 // 렌더링
	if (Collider2D())		 // 충돌체 존재시 렌더링
		Collider2D()->Render();
}

SharedPtr<CMaterial> CMeshRenderer::GetCloneMaterial()
{
	if (nullptr == m_pMtrl)
		return nullptr;
	m_pMtrl = m_pMtrl->Clone();
	return m_pMtrl;
}