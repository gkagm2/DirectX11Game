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
	m_eSkyboxType{E_SkyboxType::Cube}
{
	m_pSkyboxMaterial = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_SkyboxMtrl);

	SetSkyboxType(m_eSkyboxType);

	assert(m_pSkyboxMaterial.Get());
	// Default skkybox texture setting
	m_pSkyboxMaterial->GetData(E_ShaderParam::TextureCube_0, m_pSkyboxTexture.GetAddress());
	assert(m_pSkyboxTexture.Get());
}

CSkybox::~CSkybox()
{
}

void CSkybox::FinalUpdate()
{
}

void CSkybox::UpdateData()
{
	m_pSkyboxMaterial->SetData(E_ShaderParam::Int_0, (int*)&m_eSkyboxType);
}

void CSkybox::Render()
{
	if (nullptr == m_pSkyboxMesh  && nullptr == m_pSkyboxMaterial)
		return;

	// 위치정보 세팅
	if (Transform())
		Transform()->UpdateData();

	UpdateData();
	
	m_pSkyboxMaterial->UpdateData();	 // 메터리얼 세팅
	m_pSkyboxMesh->Render(); // 렌더링
	//m_pSphereMtrl->Clear();		 // 메터리얼 레지스터 Clear
}

bool CSkybox::SaveToScene(FILE* _pFile)
{
	CRenderer::SaveToScene(_pFile);
	SaveResourceToFile(m_pSkyboxMaterial, _pFile);
	SaveResourceToFile(m_pSkyboxMesh, _pFile);
	return true;
}

bool CSkybox::LoadFromScene(FILE* _pFile)
{
	CRenderer::LoadFromScene(_pFile);
	LoadResourceFromFile(m_pSkyboxMaterial, _pFile);
	LoadResourceFromFile(m_pSkyboxMesh, _pFile);
	return true;
}

void CSkybox::SetSkyboxTexture(SharedPtr<CTexture> _pTexture)
{
	if (_pTexture->IsCubeTexture())
		m_pSkyboxMaterial->SetData(E_ShaderParam::TextureCube_0, _pTexture.Get());
	else
		m_pSkyboxMaterial->SetData(E_ShaderParam::Texture_0, _pTexture.Get());
	m_pSkyboxTexture = _pTexture;
}

SharedPtr<CTexture> CSkybox::GetSkyboxTexture()
{
	return SharedPtr<CTexture>();
}

void CSkybox::SetSkyboxType(E_SkyboxType _eType)
{
	m_eSkyboxType = _eType;
	switch (_eType) {
	case E_SkyboxType::Cube:
		m_pSkyboxMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_CubeMesh);
		break;
	case E_SkyboxType::Sphere:
		m_pSkyboxMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_SphereMesh);

		break;
	default:
		assert(nullptr);
		break;
	}
	assert(m_pSkyboxMesh.Get());
}
