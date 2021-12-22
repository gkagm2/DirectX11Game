#include "pch.h"
#include "CButtonUI.h"
#include "CConstBuffer.h"
#include "CResourceManager.h"
#include "CMaterial.h"

CButtonUI::CButtonUI() :
	CImageUI(E_ComponentType::ButtonUI),
	m_iNormalColor{ COLOR_RGBA(255,255,255,255) },
	m_iHighlightedColor{ COLOR_RGBA(175,255,160,255) },
	m_iPressedColor{ COLOR_RGBA(150,150,150,255) },
	m_iSelectedColor{ COLOR_RGBA(150,150,150,255) },
	m_iDisableColor{ COLOR_RGBA(50,50,50,255) },
	m_fFadeDuration{ 0.1f},
	m_eButtonState{E_ButtonState::Normal}
{
	m_pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(STR_KEY_ButtonUIMtrl);
	m_pSharedMtrl = m_pMtrl;
	assert(m_pMtrl.Get());
	assert(m_pSharedMtrl.Get());

	ChangeButtonState(m_eButtonState);
}

CButtonUI::CButtonUI(const CButtonUI& _origin) :
	CImageUI(E_ComponentType::ButtonUI),
	m_iNormalColor{ _origin.m_iNormalColor},
	m_iHighlightedColor{ _origin.m_iHighlightedColor},
	m_iPressedColor{ _origin.m_iPressedColor},
	m_iSelectedColor{ _origin. m_iSelectedColor},
	m_iDisableColor{ _origin. m_iDisableColor},
	m_fFadeDuration{ _origin.m_fFadeDuration },
	m_eButtonState{ _origin.m_eButtonState }
{
	SharedPtr<CMaterial> pNewMtrl = GetCloneMaterial();
	m_pMtrl = pNewMtrl;
	m_pTexture = _origin.m_pTexture;
	ChangeButtonState(m_eButtonState);
}

CButtonUI::~CButtonUI()
{
}

void CButtonUI::FinalUpdate()
{
	switch (m_eButtonState) {
	case E_ButtonState::Normal: {
		Vector4 vNormalColorRGBA = ChangeColorUintToVector4(m_iNormalColor); // normal color
		GetCurMateial()->SetData(E_ShaderParam::Vector4_0, vNormalColorRGBA);
		SetColor(m_iNormalColor);
	}
							  break;
	case E_ButtonState::Highlighted: {
		Vector4 vHighlightedColorRGBA = ChangeColorUintToVector4(m_iHighlightedColor);
		GetCurMateial()->SetData(E_ShaderParam::Vector4_0, vHighlightedColorRGBA);
		SetColor(m_iHighlightedColor);
	}
								   break;
	case E_ButtonState::Pressed: {
		Vector4 vPressedColorRGBA = ChangeColorUintToVector4(m_iPressedColor);
		GetCurMateial()->SetData(E_ShaderParam::Vector4_0, vPressedColorRGBA);
		SetColor(m_iPressedColor);
	}
							   break;
	case E_ButtonState::Selected: {
		Vector4 vSelectedColorRGBA = ChangeColorUintToVector4(m_iSelectedColor);

		GetCurMateial()->SetData(E_ShaderParam::Vector4_0, vSelectedColorRGBA);
		SetColor(m_iSelectedColor);
	}
								break;
	case E_ButtonState::Disable: {
		Vector4 vDisableColorRGBA = ChangeColorUintToVector4(m_iDisableColor);
		GetCurMateial()->SetData(E_ShaderParam::Vector4_0, vDisableColorRGBA);
		SetColor(m_iDisableColor);
	}
							   break;
	default:
		assert(nullptr);
		break;
	}

	Vector4 vCurColor = ChangeColorUintToVector4(GetColor());
	m_pMtrl->SetData(E_ShaderParam::Vector4_0, vCurColor);
	if (nullptr != m_pTexture.Get())
		m_pMtrl->SetData(E_ShaderParam::Texture_0, m_pTexture.Get());
}


void CButtonUI::SetNormalColor(UINT _iColor) {
	if (!IsCurMtrlAlreadyClone()) {
		SharedPtr<CMaterial> pNewMtrl = GetCloneMaterial();
		m_pMtrl = pNewMtrl;
	}
	m_iNormalColor = _iColor;
}

void CButtonUI::SetHighlightedColor(UINT _iColor) {
	if (!IsCurMtrlAlreadyClone()) {
		SharedPtr<CMaterial> pNewMtrl = GetCloneMaterial();
		m_pMtrl = pNewMtrl;
	}
	m_iHighlightedColor = _iColor;
}

void CButtonUI::SetPressedColor(UINT _iColor) {
	if (!IsCurMtrlAlreadyClone()) {
		SharedPtr<CMaterial> pNewMtrl = GetCloneMaterial();
		m_pMtrl = pNewMtrl;
	}
	m_iPressedColor = _iColor;
}

void CButtonUI::SetSelectedColor(UINT _iColor) { 
	if (!IsCurMtrlAlreadyClone()) {
		SharedPtr<CMaterial> pNewMtrl = GetCloneMaterial();
		m_pMtrl = pNewMtrl;
	}
	m_iSelectedColor = _iColor;
}

void CButtonUI::SetDisableColor(UINT _iColor) {
	if (!IsCurMtrlAlreadyClone()) {
		SharedPtr<CMaterial> pNewMtrl = GetCloneMaterial();
		m_pMtrl = pNewMtrl;
	}
	m_iDisableColor = _iColor; 
}

bool CButtonUI::SaveToScene(FILE* _pFile)
{
	CImageUI::SaveToScene(_pFile);
	FWrite(m_iHighlightedColor, _pFile);
	FWrite(m_iPressedColor, _pFile);
	FWrite(m_iSelectedColor, _pFile);
	FWrite(m_iDisableColor, _pFile);
	FWrite(m_fFadeDuration,_pFile);
	return true;
}

bool CButtonUI::LoadFromScene(FILE* _pFile)
{
	CImageUI::LoadFromScene(_pFile);
	FRead(m_iHighlightedColor, _pFile);
	FRead(m_iPressedColor, _pFile);
	FRead(m_iSelectedColor, _pFile);
	FRead(m_iDisableColor, _pFile);
	FRead(m_fFadeDuration, _pFile);
	return true;
}