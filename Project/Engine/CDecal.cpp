#include "pch.h"
#include "CDecal.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CSceneManager.h"

CDecal::CDecal() :
	CRenderer(E_ComponentType::Decal),
	m_bLighting{ true }
{
	m_pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_CubeMesh);
	m_pDebugMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_DecalDebugMtrl);
	m_pDecalMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_DecalMtrl);
}

CDecal::~CDecal()
{
}

void CDecal::FinalUpdate()
{
}

void CDecal::Render()
{
	Transform()->UpdateData();

	if (nullptr != m_pTexture)
		m_pDecalMtrl->SetData(E_ShaderParam::Texture_1, m_pTexture.Get());
	m_pDecalMtrl->SetData(E_ShaderParam::Int_0, (void*)&m_bLighting);

	m_pDecalMtrl->UpdateData();
	m_pMesh->Render();

	if (E_SceneMode::Play != CSceneManager::GetInstance()->GetSceneMode()) {
		m_pDebugMtrl->UpdateData();
		m_pMesh->Render();
	}
}

bool CDecal::SaveToScene(FILE* _pFile)
{
	CRenderer::SaveToScene(_pFile);
	FWrite(m_bLighting, _pFile);
	SaveResourceToFile(m_pTexture, _pFile);
	return true;
}

bool CDecal::LoadFromScene(FILE* _pFile)
{
	CRenderer::LoadFromScene(_pFile);
	FRead(m_bLighting, _pFile);
	LoadResourceFromFile(m_pTexture, _pFile);
	return true;
}
