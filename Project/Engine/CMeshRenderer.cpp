#include "pch.h"
#include "CMesh.h"
#include "CResourceManager.h"
#include "CMeshRenderer.h"
#include "CGraphicsShader.h"

#include "CTexture.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CAnimator2D.h"
#include "CMeshRenderer.h"

CMeshRenderer::CMeshRenderer() :
	CRenderer(E_ComponentType::MeshRenderer),
	m_pMesh(nullptr),
	m_pMtrl(nullptr)
{
	m_pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMesh);
	m_pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_StdAlphaBlend_CoverageMtrl);
}

CMeshRenderer::~CMeshRenderer()
{
}

void CMeshRenderer::Render()
{
	if(nullptr == m_pMesh || nullptr == m_pMtrl)
		return;

	Transform()->UpdateData(); // 위치정보 세팅
	
	if (Animator2D())
		Animator2D()->UpdateData();
	else
		CAnimator2D::Clear();


	m_pMesh->UpdateData();   // 메쉬 세팅
	m_pMtrl->UpdateData();	 // 메터리얼 세팅
	m_pMesh->Render();		 // 렌더링

	m_pMtrl->Clear();		 // 메터리얼 레지스터 Clear
}

SharedPtr<CMaterial> CMeshRenderer::GetCloneMaterial()
{
	if (nullptr == m_pMtrl)
		return nullptr;
	m_pMtrl = m_pMtrl->Clone();
	return m_pMtrl;
}

void CMeshRenderer::SetTexSize(const Vector2& _vSize)
{
}

const Vector2& CMeshRenderer::GetTexSize()
{
	SharedPtr<CTexture> pTexture = nullptr;
	m_pMtrl->GetData(E_ShaderParam::Texture_0, pTexture.Get());
	return pTexture->GetDimension();
}

bool CMeshRenderer::SaveToScene(FILE* _pFile)
{
	SaveResourceToFile(m_pMesh, _pFile);
	SaveResourceToFile(m_pMtrl, _pFile);
	return true;
}

bool CMeshRenderer::LoadFromScene(FILE* _pFile)
{					
	LoadResourceFromFile(m_pMesh, _pFile);
	LoadResourceFromFile(m_pMtrl, _pFile);
	return true;
}