#include "pch.h"
#include "CButtonUI.h"

CButtonUI::CButtonUI() :
	CImageUI(E_ComponentType::ButtonUI),
	m_iNormalColor{ COLOR_RGBA(0,0,0,255)},
	m_iHighlightedColor{ COLOR_RGBA(0,0,0,255) },
	m_iPressedColor{ COLOR_RGBA(0,0,0,255) },
	m_iSelectedColor{ COLOR_RGBA(0,0,0,255) },
	m_iDisableColor{ COLOR_RGBA(0,0,0,255) },
	m_fFadeDuration{ 0.1f}
{
}

CButtonUI::~CButtonUI()
{
}

void CButtonUI::FinalUpdate()
{
	SharedPtr<CMaterial> pMtrl = GetCloneMaterial();
	Vector4 colf = ChangeColorUintToVector4(m_iNormalColor);
	pMtrl->SetData(E_ShaderParam::Vector4_0, &colf);
}

void CButtonUI::UpdateData()
{
	// TODO (Jang) : GPU 버퍼로 보내기
}

void CButtonUI::Render()
{
	CImageUI::Render();
}

bool CButtonUI::SaveToScene(FILE* _pFile)
{
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
	FRead(m_iNormalColor, _pFile);
	FRead(m_iHighlightedColor, _pFile);
	FRead(m_iPressedColor, _pFile);
	FRead(m_iSelectedColor, _pFile);
	FRead(m_iDisableColor, _pFile);
	FRead(m_fFadeDuration, _pFile);
	return true;
}