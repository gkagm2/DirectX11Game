#include "pch.h"
#include "CButtonUI.h"

CButtonUI::CButtonUI() :
	CImageUI(E_ComponentType::ButtonUI),
	m_fNormalColor{ Vector4(0.f,0.f,0.f,1.f) },
	m_fHighlightedColor{ Vector4(0.f,0.f,0.f,1.f) },
	m_fPressedColor{ Vector4(0.f,0.f,0.f,1.f) },
	m_fSelectedColor{ Vector4(0.f,0.f,0.f,1.f) },
	m_fDisableColor{ Vector4(0.f,0.f,0.f,1.f) },
	m_fFadeDuration{ 0.1f}
{
}

CButtonUI::~CButtonUI()
{
}

void CButtonUI::FinalUpdate()
{
}

void CButtonUI::UpdateData()
{
}

void CButtonUI::Render()
{
}

bool CButtonUI::SaveToScene(FILE* _pFile)
{
	FWrite(m_fNormalColor, _pFile);
	FWrite(m_fHighlightedColor, _pFile);
	FWrite(m_fPressedColor, _pFile);
	FWrite(m_fSelectedColor, _pFile);
	FWrite(m_fDisableColor, _pFile);
	FWrite(m_fFadeDuration,_pFile);
	return true;
}

bool CButtonUI::LoadFromScene(FILE* _pFile)
{
	FRead(m_fNormalColor, _pFile);
	FRead(m_fHighlightedColor, _pFile);
	FRead(m_fPressedColor, _pFile);
	FRead(m_fSelectedColor, _pFile);
	FRead(m_fDisableColor, _pFile);
	FRead(m_fFadeDuration, _pFile);
	return true;
}