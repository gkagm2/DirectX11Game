#include "pch.h"
#include "CButtonUI.h"
#include "CConstBuffer.h"

CButtonUI::CButtonUI() :
	CImageUI(E_ComponentType::ButtonUI),
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
	Vector4 vNormalColorRGBA = ChangeColorUintToVector4(GetColor()); // normal color
	Vector4 vHighlightedColorRGBA = ChangeColorUintToVector4(m_iHighlightedColor);

	Vector4 vPressedColorRGBA = ChangeColorUintToVector4(m_iPressedColor);
	Vector4 vSelectedColorRGBA = ChangeColorUintToVector4(m_iSelectedColor);

	GetSharedMaterial()->SetData(E_ShaderParam::Vector4_0, &vNormalColorRGBA);
	GetSharedMaterial()->SetData(E_ShaderParam::Vector4_1, &vHighlightedColorRGBA);
	GetSharedMaterial()->SetData(E_ShaderParam::Vector4_2, &vPressedColorRGBA);
	GetSharedMaterial()->SetData(E_ShaderParam::Vector4_3, &vSelectedColorRGBA);

	Vector4 vDisableColorRGBA = ChangeColorUintToVector4(m_iDisableColor);
	Vector2 vDisableColorRG = Vector2(vDisableColorRGBA.x, vDisableColorRGBA.y);
	Vector2 vDisableColorBA = Vector2(vDisableColorRGBA.z, vDisableColorRGBA.w);
	GetSharedMaterial()->SetData(E_ShaderParam::Vector2_0, &vDisableColorRG);
	GetSharedMaterial()->SetData(E_ShaderParam::Vector2_1, &vDisableColorBA);


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