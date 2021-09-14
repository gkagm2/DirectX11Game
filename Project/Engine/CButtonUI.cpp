#include "pch.h"
#include "CButtonUI.h"

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
}

CButtonUI::~CButtonUI()
{
}

void CButtonUI::FinalUpdate()
{
	UINT iColor = GetColor();
	Vector4 vColorRGBA = ChangeColorUintToVector4(iColor); // 색상을 Vector4로 변환
	GetSharedMaterial()->SetData(E_ShaderParam::Vector4_0, vColorRGBA);
}

void CButtonUI::UpdateData()
{
	CImageUI::UpdateData();

	switch(m_eButtonState) {
	case E_ButtonState::Normal:
		SetColor(m_iNormalColor);
		break;
	case E_ButtonState::Highlighted:
		SetColor(m_iHighlightedColor);
		break;
	case E_ButtonState::Pressed:
		SetColor(m_iPressedColor);
		break;
	case E_ButtonState::Selected:
		SetColor(m_iSelectedColor);
		break;
	case E_ButtonState::Disable:
		SetColor(m_iDisableColor);
		break;
	default:
		assert(nullptr);
		break;
	}
}

void CButtonUI::Render()
{
	CImageUI::Render();
}

bool CButtonUI::SaveToScene(FILE* _pFile)
{
	CImageUI::SaveToScene(_pFile);
	FWrite(m_iNormalColor, _pFile);
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
	FRead(m_iNormalColor, _pFile);
	FRead(m_iHighlightedColor, _pFile);
	FRead(m_iPressedColor, _pFile);
	FRead(m_iSelectedColor, _pFile);
	FRead(m_iDisableColor, _pFile);
	FRead(m_fFadeDuration, _pFile);
	return true;
}