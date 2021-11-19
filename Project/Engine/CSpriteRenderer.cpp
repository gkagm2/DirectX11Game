#include "pch.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CTransform.h"
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
	if (nullptr != m_pCloneMaterial.Get()) {
		delete m_pCloneMaterial.Get();
		m_pCloneMaterial = nullptr;
	}
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

	// ��ġ���� ����
	if (Transform())
		Transform()->UpdateData();

	if (Animator2D())
		Animator2D()->UpdateData();
	else
		CAnimator2D::Clear();


	m_pMtrl->UpdateData();	 // ���͸��� ����
	m_pMesh->Render();		 // ������

	m_pMtrl->Clear();		 // ���͸��� �������� Clear
}