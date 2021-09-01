#include "pch.h"
#include "TextUIGUI.h"
#include <Engine\CFontManager.h>
#include <Engine\CTextUI.h>

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
	ImGui::Text("Color");

	UINT iColor = pTextUI->GetColor();
	BYTE br = FONT_R_FROM_RGBA(iColor);
	BYTE bg = FONT_G_FROM_RGBA(iColor);
	BYTE bb = FONT_B_FROM_RGBA(iColor);
	BYTE ba = FONT_A_FROM_RGBA(iColor);

	static ImVec4 colf = ImVec4(float(br / 255.f), float(bg / 255.f), float(bb / 255.f), float(ba / 255.f));

	if (ImGui::ColorEdit4("Color", &colf.x, ImGuiColorEditFlags_InputRGB)) {
		BYTE r = BYTE(colf.x * 255);
		BYTE g = BYTE(colf.y * 255);
		BYTE b = BYTE(colf.z * 255);
		BYTE a = BYTE(colf.w * 255);
		iColor = FONT_RGBA(r, g, b, a);
	}
	pTextUI->SetColor(iColor);

	End();
}