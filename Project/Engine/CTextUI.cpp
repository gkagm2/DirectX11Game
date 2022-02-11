#include "pch.h"
#include "CTextUI.h"
#include "CFontManager.h"
#include "CRenderManager.h"
#include "CRectTransform.h"
#include "CCamera.h"
#include "CScript.h"

CTextUI::CTextUI() :
	CUI(E_ComponentType::TextUI),
	m_strText(_T("")),
	m_fFontSize(10.f),
	m_vScreenPos{},
	m_iColor(FONT_RGBA(0, 0, 0, 255))
{
	SetAlign(E_TextAlign::Left);
	SetFontType(0);
}

CTextUI::~CTextUI()
{
}

void CTextUI::FinalUpdate()
{

}

void CTextUI::UpdateData()
{
}

void CTextUI::Render()
{
	wstring strText;
	TStringToWString(m_strText, strText);

	CCamera* pUICamera = CRenderManager::GetInstance()->GetUICamera();
	if (!pUICamera) {
		assert(pUICamera);
		return;
	}
	if (E_ProjectionType::Orthographic == pUICamera->GetProjectionType()) {
		m_vScreenPos = pUICamera->GetWorldToScreen2DPosition(RectTransform()->GetPosition());
	}
	float fFontSize = GetFontSize();
	if (0.f != fFontSize) {
		if (m_tFontType.strName.empty()) {
			CFontManager::GetInstance()->DrawFont(strText.c_str(), m_vScreenPos.x, m_vScreenPos.y, fFontSize, m_iColor, m_eFlag);
		}
		else {
			CFontManager::GetInstance()->DrawFont1(strText.c_str(), m_tFontType.strName.c_str(), m_vScreenPos.x, m_vScreenPos.y, fFontSize, m_iColor, m_eFlag);
		}
	}

	CUI::Render();
}

void CTextUI::SetFontType(int iIdx)
{
	if (iIdx < 0 || iIdx >= CFontManager::GetInstance()->GetFontList().size()) {
		assert(nullptr);
		return;
	}
	m_tFontType = CFontManager::GetInstance()->GetFontList()[iIdx];
}


void CTextUI::SetAlign(E_TextAlign _eTextAlignment)
{
	m_eTextAlignment = _eTextAlignment;
	switch (_eTextAlignment) {
	case E_TextAlign::LeftTop:
		m_eFlag = FW1_TEXT_FLAG(FW1_TEXT_FLAG::FW1_LEFT | FW1_TEXT_FLAG::FW1_TOP);
		m_strTextAlignment = _T("Left Top");
		break;
	case E_TextAlign::MiddleTop:
		m_eFlag = FW1_TEXT_FLAG(FW1_TEXT_FLAG::FW1_CENTER | FW1_TEXT_FLAG::FW1_TOP);
		m_strTextAlignment = _T("Middle Top");
		break;
	case E_TextAlign::RightTop:
		m_eFlag = FW1_TEXT_FLAG(FW1_TEXT_FLAG::FW1_RIGHT | FW1_TEXT_FLAG::FW1_TOP);
		m_strTextAlignment = _T("Right Top");
		break;
	case E_TextAlign::Left:
		m_eFlag = FW1_TEXT_FLAG(FW1_TEXT_FLAG::FW1_LEFT | FW1_TEXT_FLAG::FW1_VCENTER);
		m_strTextAlignment = _T("Left");
		break;
	case E_TextAlign::Center:
		m_eFlag = FW1_TEXT_FLAG(FW1_TEXT_FLAG::FW1_CENTER | FW1_TEXT_FLAG::FW1_VCENTER);
		m_strTextAlignment = _T("Center");
		break;
	case E_TextAlign::Right:
		m_eFlag = FW1_TEXT_FLAG(FW1_TEXT_FLAG::FW1_RIGHT | FW1_TEXT_FLAG::FW1_VCENTER);
		m_strTextAlignment = _T("Right");
		break;
	case E_TextAlign::LeftBottom:
		m_eFlag = FW1_TEXT_FLAG(FW1_TEXT_FLAG::FW1_LEFT | FW1_TEXT_FLAG::FW1_BOTTOM);
		m_strTextAlignment = _T("Left Bottom");
		break;
	case E_TextAlign::MiddleBottom:
		m_eFlag = FW1_TEXT_FLAG(FW1_TEXT_FLAG::FW1_CENTER | FW1_TEXT_FLAG::FW1_BOTTOM);
		m_strTextAlignment = _T("Middle Bottom");
		break;
	case E_TextAlign::RightBottom:
		m_eFlag = FW1_TEXT_FLAG(FW1_TEXT_FLAG::FW1_RIGHT | FW1_TEXT_FLAG::FW1_BOTTOM);
		m_strTextAlignment = _T("Right Bottom");
		break;
	default:
		assert(nullptr);
		break;
	}
}

bool CTextUI::SaveToScene(FILE* _pFile)
{
	CUI::SaveToScene(_pFile);
	SaveStringToFile(m_strText, _pFile);
	FWrite(m_fFontSize, _pFile);
	FWrite(m_iColor, _pFile);
	FWrite(m_eTextAlignment, _pFile);
	m_tFontType.Save(_pFile);

	return true;
}

bool CTextUI::LoadFromScene(FILE* _pFile)
{
	CUI::LoadFromScene(_pFile);
	LoadStringFromFile(m_strText, _pFile);
	FRead(m_fFontSize, _pFile);
	FRead(m_iColor, _pFile);
	FRead(m_eTextAlignment, _pFile);
	SetAlign(m_eTextAlignment);
	m_tFontType.Load(_pFile);

	return true;
}