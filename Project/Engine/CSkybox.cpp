#include "pch.h"
#include "CSkybox.h"
#include "CResourceManager.h"
#include "CTransform.h"
#include "CAnimation2D.h"

tstring SkyboxTypeToStr(E_SkyboxType _eType)
{
	tstring strType{};
	switch (_eType) {
	case E_SkyboxType::Cube:
		strType = _T("Cube");
		break;
	case E_SkyboxType::Sphere:
		strType = _T("Sphere");
		break;
	default:
		assert(nullptr);
		break;
	}
	return strType;
}

CSkybox::CSkybox() :
	CRenderer(E_ComponentType::Skybox),
	m_eSkyboxType{E_SkyboxType::Sphere}
{
	m_pSphereMaterial = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_SkyboxMtrl);
	// TOOD : Cube Mtrl
	m_pCubeMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_CubeMesh);
	m_pSphereMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_SphereMesh);

	assert(m_pSphereMaterial.Get());
	assert(m_pCubeMesh.Get());
	assert(m_pSphereMesh.Get());
}

CSkybox::~CSkybox()
{
}

void CSkybox::FinalUpdate()
{
}

void CSkybox::UpdateData()
{
	m_pSphereMaterial->SetData(E_ShaderParam::Int_0, (int*)&m_eType);
}

void CSkybox::Render()
{
	if (nullptr == m_pCubeMesh && nullptr == m_pSphereMesh  && nullptr == m_pSphereMaterial)
		return;

	// 위치정보 세팅
	if (Transform())
		Transform()->UpdateData();

	UpdateData();
	
	m_pSphereMaterial->UpdateData();	 // 메터리얼 세팅
	m_pSphereMesh->Render(); // 렌더링
	//m_pSphereMtrl->Clear();		 // 메터리얼 레지스터 Clear
}