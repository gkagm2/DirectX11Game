#include "pch.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CAnimation2D.h"
#include "CAnimation2D.h"

CSpriteRenderer::CSpriteRenderer() :
	CRenderer(E_ComponentType::SpriteRenderer)
{
	m_pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMesh);
	m_pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdLight2DMtrl);
	m_pSharedMaterial = m_pMtrl;
}

CSpriteRenderer::~CSpriteRenderer()
{
	SAFE_DELETE_PTR(m_pCloneMaterial);
}

SharedPtr<CMaterial> CSpriteRenderer::GetCloneMaterial()
{
	if (nullptr == m_pMtrl)
		return nullptr;
	m_pMtrl = m_pMtrl->Clone();
	return m_pMtrl;
}

void CSpriteRenderer::Render()
{
	if (nullptr == m_pMesh || nullptr == m_pMtrl)
		return;

	// 위치정보 세팅
	if (Transform())
		Transform()->UpdateData();

	if (Animator2D())
		Animator2D()->UpdateData();
	else
		CAnimation2D::Clear();


	m_pMtrl->UpdateData();	 // 메터리얼 세팅
	m_pMesh->Render();		 // 렌더링

	m_pMtrl->Clear();		 // 메터리얼 레지스터 Clear
}