#include "pch.h"
#include "CImageUI.h"
#include "CCanvasRenderer.h"
#include "CRectTransform.h"
#include "CMesh.h"
#include "CResourceManager.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CMaterial.h"

CImageUI::CImageUI() :
	CUI(E_ComponentType::ImageUI),
	m_iColor{COLOR_RGBA(255,255,255,255)}
{
	// �ϴ� �⺻�� CanvasMaterial�� �̿��Ѵ�.
	m_pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMesh);
	m_pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_CanvasMtrl);
	m_pSharedMtrl = m_pMtrl;
}

CImageUI::CImageUI(const CImageUI& _other) :
	CUI(E_ComponentType::ImageUI),
	m_iColor{ _other.m_iColor}
{
	m_pMesh = _other.m_pMesh;
	m_pSharedMtrl = _other.m_pSharedMtrl;
	m_pMtrl = _other.m_pMtrl;
	m_pMtrl = m_pMtrl.Get()->CloneDeep();
	m_pCloneMtrl = m_pMtrl;
}

CImageUI::CImageUI(E_ComponentType _eType) :
	CUI(_eType),
	m_iColor{ COLOR_RGBA(255,255,255,255) }
{
	// �ϴ� �⺻�� CanvasMaterial�� �̿��Ѵ�.
	m_pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(STR_KEY_RectMesh);
	m_pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_CanvasMtrl);
	m_pSharedMtrl = m_pMtrl;
}

CImageUI::~CImageUI()
{
	if (nullptr != m_pCloneMtrl) {
		delete m_pCloneMtrl.Get();
		m_pCloneMtrl = nullptr;
	}
}

void CImageUI::FinalUpdate()
{
	Vector4 vColorRGBA = ChangeColorUintToVector4(m_iColor); // ������ Vector4�� ��ȯ
	m_pMtrl->SetData(E_ShaderParam::Vector4_0, vColorRGBA);
	if(nullptr != m_pTexture.Get())
		m_pMtrl->SetData(E_ShaderParam::Texture_0, m_pTexture.Get());
}

void CImageUI::Render()
{
	if (nullptr == m_pMesh || nullptr == m_pMtrl)
	return;

	// ��ġ���� ����
	RectTransform()->UpdateData();

	m_pMtrl->UpdateData();	 // ���͸��� ����
	m_pMesh->Render();		 // ������

	m_pMtrl->Clear();		 // ���͸��� �������� Clear

	CUI::Render();
}

void CImageUI::SetImageTex(SharedPtr<CTexture> _pTexture)
{
	if (nullptr != m_pCloneMtrl) {
		delete m_pCloneMtrl.Get();
		m_pCloneMtrl = nullptr;
	}
	if (nullptr == _pTexture)
		m_pMtrl = m_pSharedMtrl;

	m_pCloneMtrl = GetCloneMaterial();
	m_pMtrl = m_pCloneMtrl;
	m_pTexture = _pTexture;
}

void CImageUI::SetColor(UINT _iColor)
{
	GetCloneMaterial(); // ���������� clone�� ������ ���� ������ �־���
	m_iColor = _iColor;
}

SharedPtr<CTexture> CImageUI::GetImageTex()
{
	return m_pTexture;
}

SharedPtr<CMaterial> CImageUI::GetCloneMaterial()
{
	if (nullptr == m_pSharedMtrl)
		return nullptr;
	if(nullptr == m_pCloneMtrl) {
		m_pCloneMtrl = m_pSharedMtrl->CloneDeep();
		m_pMtrl = m_pCloneMtrl;
	}
	else {
		m_pMtrl = m_pCloneMtrl->CloneDeep();
		delete m_pCloneMtrl.Get();
		m_pCloneMtrl = nullptr;
		m_pCloneMtrl = m_pMtrl;
	}
	return m_pMtrl;
}

SharedPtr<CMaterial> CImageUI::GetSharedMaterial() {
	m_pMtrl = m_pSharedMtrl;
	return m_pMtrl;
}

bool CImageUI::SaveToScene(FILE* _pFile)
{
	// CanvasRenderer���� �⺻ material�� mesh�� ���������� �׷�����
	CUI::SaveToScene(_pFile);
	SaveResourceToFile(m_pTexture, _pFile);
	SaveResourceToFile(m_pMtrl, _pFile);
	SaveResourceToFile(m_pMesh, _pFile);
	SaveResourceToFile(m_pCloneMtrl, _pFile);
	SaveResourceToFile(m_pSharedMtrl, _pFile);
	FWrite(m_iColor, _pFile);
	return true;
}

bool CImageUI::LoadFromScene(FILE* _pFile)
{
	CUI::LoadFromScene(_pFile);
	LoadResourceFromFile(m_pTexture, _pFile);
	LoadResourceFromFile(m_pMtrl, _pFile);
	LoadResourceFromFile(m_pMesh, _pFile);
	LoadResourceFromFile(m_pCloneMtrl, _pFile);
	LoadResourceFromFile(m_pSharedMtrl, _pFile);
	FRead(m_iColor, _pFile);
	SetImageTex(m_pTexture);
	return true;
}


bool CImageUI::IsCurMtrlAlreadyClone()
{
	// Ŭ���� �����ϸ� ���� ���͸����� Ŭ�� ���͸����̸�
	if (m_pMtrl == m_pCloneMtrl) {
		return true;
	}
	return false;
}