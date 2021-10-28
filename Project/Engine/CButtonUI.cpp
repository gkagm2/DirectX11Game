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
}


void CButtonUI::SetNormalColor(UINT _iColor) {
	if (!IsCurMtrlAlreadyClone()) {
		if (nullptr == m_pCloneMtrl)
			GetCloneMaterial(); // 내부적으로 clone된 재질을 현재 재질로 넣어줌
	}
	m_iNormalColor = _iColor;
}

void CButtonUI::SetHighlightedColor(UINT _iColor) {
	if (!IsCurMtrlAlreadyClone()) {
		if (nullptr == m_pCloneMtrl)
			GetCloneMaterial(); // 내부적으로 clone된 재질을 현재 재질로 넣어줌
	}
	m_iHighlightedColor = _iColor;
}

void CButtonUI::SetPressedColor(UINT _iColor) {
	if (!IsCurMtrlAlreadyClone()) {
		if (nullptr == m_pCloneMtrl)
			GetCloneMaterial(); // 내부적으로 clone된 재질을 현재 재질로 넣어줌
	}
	m_iPressedColor = _iColor;
}

void CButtonUI::SetSelectedColor(UINT _iColor) { 
	if (!IsCurMtrlAlreadyClone()) {
		if (nullptr == m_pCloneMtrl)
			GetCloneMaterial(); // 내부적으로 clone된 재질을 현재 재질로 넣어줌
	}
	m_iSelectedColor = _iColor;
}

void CButtonUI::SetDisableColor(UINT _iColor) {
	if (!IsCurMtrlAlreadyClone()) {
		if (nullptr == m_pCloneMtrl)
			GetCloneMaterial(); // 내부적으로 clone된 재질을 현재 재질로 넣어줌
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