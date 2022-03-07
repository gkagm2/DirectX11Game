#include "pch.h"
#include "CDecal.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CSceneManager.h"

CDecal::CDecal() :
	CRenderer(E_ComponentType::Decal),
	m_bLighting{ false }
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
	UpdateData();

	m_pMesh->Render();

	if (E_SceneMode::Play != CSceneManager::GetInstance()->GetSceneMode()) {
		m_pDebugMtrl->UpdateData();
		m_pMesh->Render();
	}
}

void CDecal::UpdateData()
{
	Transform()->UpdateData();

	if (nullptr != m_pTexture)
		m_pDecalMtrl->SetData(E_ShaderParam::Texture_0, m_pTexture.Get());

	m_pDecalMtrl->UpdateData();
	m_pDebugMtrl->UpdateData();
}