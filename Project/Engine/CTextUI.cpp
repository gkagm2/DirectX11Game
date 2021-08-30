#include "pch.h"
#include "CTextUI.h"
#include "CFontManager.h"
#include "CRenderManager.h"
#include "CRectTransform.h"

CTextUI::CTextUI() :
	CUI(E_ComponentType::TextUI),
	m_strText(_T("")),
	m_fFontSize(10.f),
	m_vScreenPos{}
{
	m_iColor = FONT_RGBA(0, 0, 0, 255);
	SetAlign(E_TextAlign::Left);
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

	// World position을 Screen 포지션으로 바꿔야 한다.
	// TOOD (Jang) : Main Camera가 아닌 UI Camera로 해야 함
	CCamera* pUICamera = CRenderManager::GetInstance()->GetMainCamera();
	if (!pUICamera)
		return;

	if (E_ProjectionType::Orthographic == pUICamera->GetProjectionType()) {
		m_vScreenPos = pUICamera->GetWorldToScreen2DPosition(RectTransform()->GetPosition());
	}

	CFontManager::GetInstance()->DrawFont(strText.c_str(), m_vScreenPos.x, m_vScreenPos.y, GetFontSize(), m_iColor, m_eFlag);
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