#include "pch.h"
#include "CImageUI.h"
#include "CCanvasRenderer.h"
#include "CRectTransform.h"
#include "CMesh.h"
#include "CResourceManager.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CImageUI::CImageUI() :
	CUI(E_ComponentType::ImageUI),
	m_iColor{COLOR_RGBA(255,255,255,255)}
{
	// 일단 기본은 CanvasMaterial을 이용한다.
	m_pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMesh);
	m_pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_CanvasMtrl);
}

CImageUI::CImageUI(E_ComponentType _eType) :
	CUI(_eType),
	m_iColor{ COLOR_RGBA(255,255,255,255) }
{
	// 일단 기본은 CanvasMaterial을 이용한다.
	m_pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMesh);
	m_pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_CanvasMtrl);
}

CImageUI::~CImageUI()
{
}

void CImageUI::FinalUpdate()
{
	Vector4 vColorRGBA = ChangeColorUintToVector4(m_iColor); // 색상을 Vector4로 변환
	m_pMtrl->SetData(E_ShaderParam::Vector4_0, vColorRGBA);
}

void CImageUI::UpdateData()
{
}

void CImageUI::Render()
{
	if (nullptr == m_pMesh || nullptr == m_pMtrl)
	return;

	// 위치정보 세팅
	RectTransform()->UpdateData();

	m_pMesh->UpdateData();   // 메쉬 세팅
	m_pMtrl->UpdateData();	 // 메터리얼 세팅
	m_pMesh->Render();		 // 렌더링

	m_pMtrl->Clear();		 // 메터리얼 레지스터 Clear

	CUI::Render();
}

void CImageUI::SetImageTex(SharedPtr<CTexture> _pTexture)
{
	m_pMtrl = GetCloneMaterial();
	m_pMtrl->SetData(E_ShaderParam::Texture_0, _pTexture.Get());
	m_pTexture = _pTexture;
}

SharedPtr<CTexture> CImageUI::GetImageTex()
{
	return m_pTexture;
}

SharedPtr<CMaterial> CImageUI::GetCloneMaterial()
{
	if (nullptr == m_pMtrl)
		return nullptr;
	m_pMtrl = m_pMtrl->Clone();
	return m_pMtrl;
}

bool CImageUI::SaveToScene(FILE* _pFile)
{
	// CanvasRenderer에서 기본 material과 mesh를 가져왔으면 그려야지

	SaveResourceToFile(m_pTexture, _pFile);
	SaveResourceToFile(m_pMtrl, _pFile);
	SaveResourceToFile(m_pMesh, _pFile);
	FWrite(m_iColor, _pFile);
	return true;
}

bool CImageUI::LoadFromScene(FILE* _pFile)
{
	LoadResourceFromFile(m_pTexture, _pFile);
	LoadResourceFromFile(m_pMtrl, _pFile);
	LoadResourceFromFile(m_pMesh, _pFile);
	FRead(m_iColor, _pFile);
	SetImageTex(m_pTexture);
	return true;
}