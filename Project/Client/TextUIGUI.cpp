#include "pch.h"
#include "TextUIGUI.h"
#include <Engine\CFontManager.h>
#include <Engine\CTextUI.h>
#include "ParamGUI.h"

const size_t TextUIGUI::m_gTextBuffSize = 2048;

TextUIGUI::TextUIGUI() :
	ComponentGUI(E_ComponentType::TextUI),
	m_fFontScrollSpeed(0.5f),
	m_fFontSizeMin{ 0.f },
	m_fFontSizeMax{FLT_MAX}
{
}

TextUIGUI::~TextUIGUI()
{
}

void TextUIGUI::Update()
{
	if (false == Start())
		return;

	CTextUI* pTextUI = GetTargetObject()->TextUI();

	float fFontSize = pTextUI->GetFontSize();
	tstring tstrText = pTextUI->GetText();
	string strText{};
	TStringToString(tstrText, strText);

	// Text
	ImGui::Text("Text"); ImGui::SameLine();
	static char szTextBuff[(UINT)m_gTextBuffSize] = {};
	char szBuff[(UINT)m_gTextBuffSize] = {};
	sprintf_s(szBuff, m_gTextBuffSize, "%s", strText.c_str());
	if (ImGui::InputText("##TextInputUIGUI", szBuff, m_gTextBuffSize)) {
		StringToTString(szBuff, tstrText);
		pTextUI->SetText(tstrText);
	}

	// Font Align
	E_TextAlign eFlag = pTextUI->GetAlign();
	tstring tstrTextAlign = pTextUI->GetAlignText();
	string strTextAlgin;
	TStringToString(tstrTextAlign, strTextAlgin);

	ImGui::Text("Pivot %s", strTextAlgin.c_str());

	
	if (ImGui::Button("LT"))
		eFlag = E_TextAlign::LeftTop;
	ImGui::SameLine();
	if (ImGui::Button("MT"))
		eFlag = E_TextAlign::MiddleTop;
	ImGui::SameLine();
	if (ImGui::Button("RT"))
		eFlag = E_TextAlign::RightTop;
	if (ImGui::Button("L "))
		eFlag = E_TextAlign::Left;
	ImGui::SameLine();
	if (ImGui::Button("C "))
		eFlag = E_TextAlign::Center;
	ImGui::SameLine();
	if (ImGui::Button("R "))
		eFlag = E_TextAlign::Right;
	if (ImGui::Button("LB"))
		eFlag = E_TextAlign::LeftBottom;
	ImGui::SameLine();
	if (ImGui::Button("MB"))
		eFlag = E_TextAlign::MiddleBottom;
	ImGui::SameLine();
	if (ImGui::Button("RB"))
		eFlag = E_TextAlign::RightBottom;
	pTextUI->SetAlign(eFlag);

	// Font Size
	ImGui::Text("FontSize"); ImGui::SameLine();
	if (ImGui::DragFloat("", &fFontSize, m_fFontScrollSpeed, m_fFontSizeMin, m_fFontSizeMax, "%.2f")) {
		pTextUI->SetFontSize(fFontSize);
	}

	// Color
	ImGui::Text("Color##TextUIGUI");

	UINT iColor = pTextUI->GetColor();
	ParamGUI::Render_Color("Color##TextUIGUI", &iColor);
	pTextUI->SetColor(iColor);

	End();
}