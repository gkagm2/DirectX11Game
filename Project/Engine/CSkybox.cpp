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
	m_pSkyboxMaterial = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_SkyboxMtrl);

	SetSkyboxType(m_eSkyboxType);

	assert(m_pSkyboxMaterial.Get());
	// Default skkybox texture setting
	SetSkyboxTexture(CResourceManager::GetInstance()->LoadRes<CTexture>(STR_PATH_Skybox1).Get());
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

	// ��ġ���� ����
	if (Transform())
		Transform()->UpdateData();

	UpdateData();
	
	m_pSkyboxMaterial->UpdateData();	 // ���͸��� ����
	m_pSkyboxMesh->Render(); // ������
	//m_pSphereMtrl->Clear();		 // ���͸��� �������� Clear
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
