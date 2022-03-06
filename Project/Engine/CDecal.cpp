#include "pch.h"
#include "CDecal.h"
#include "CResourceManager.h"
#include "CTransform.h"

CDecal::CDecal() :
	CRenderer(E_ComponentType::Decal)
{
	m_pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_CubeMesh);
	assert(nullptr);
	// TODO (Jang) : 데칼 디버스 메터리얼 가져오기
	//m_pDebugMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>
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
}

void CDecal::UpdateData()
{
	Transform()->UpdateData();
	m_pDebugMtrl->UpdateData();
}