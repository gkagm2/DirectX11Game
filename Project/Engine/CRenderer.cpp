#include "pch.h"
#include "CRenderer.h"

#include "CMesh.h"
#include "CResourceManager.h"
#include "CGraphicsShader.h"

#include "CTexture.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CRectTransform.h"
#include "CAnimator2D.h"
CRenderer::CRenderer(E_ComponentType _eType) :
	CComponent(_eType),
	m_pMesh(nullptr),
	m_pMtrl(nullptr),
	m_iColor{COLOR_RGBA(0,0,0,255)}
{
}

CRenderer::~CRenderer()
{
}

void CRenderer::Render()
{
	if (nullptr == m_pMesh || nullptr == m_pMtrl)
		return;

	// 위치정보 세팅
	if (RectTransform())
		RectTransform()->UpdateData();
	else
		Transform()->UpdateData();

	if (Animator2D())
		Animator2D()->UpdateData();
	else
		CAnimator2D::Clear();


	m_pMesh->UpdateData();   // 메쉬 세팅
	m_pMtrl->UpdateData();	 // 메터리얼 세팅
	m_pMesh->Render();		 // 렌더링

	m_pMtrl->Clear();		 // 메터리얼 레지스터 Clear
}

SharedPtr<CMaterial> CRenderer::GetCloneMaterial()
{
	if (nullptr == m_pMtrl)
		return nullptr;
	m_pMtrl = m_pMtrl->Clone();
	return m_pMtrl;
}

void CRenderer::SetTexSize(const Vector2& _vSize)
{
}

const Vector2& CRenderer::GetTexSize()
{
	SharedPtr<CTexture> pTexture = nullptr;
	m_pMtrl->GetData(E_ShaderParam::Texture_0, pTexture.GetAddress());
	return pTexture->GetDimension();
}

bool CRenderer::SaveToScene(FILE* _pFile)
{
	SaveResourceToFile(m_pMesh, _pFile);
	SaveResourceToFile(m_pMtrl, _pFile);
	return true;
}

bool CRenderer::LoadFromScene(FILE* _pFile)
{
	LoadResourceFromFile(m_pMesh, _pFile);
	LoadResourceFromFile(m_pMtrl, _pFile);
	return true;
}