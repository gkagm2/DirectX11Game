#include "pch.h"
#include "TextUIGUI.h"
#include <Engine\CFontManager.h>

TextUIGUI::TextUIGUI() :
	ComponentGUI(E_ComponentType::TextUI),
	m_strContent(""),
	m_vPos{},
	m_fFontSize(10.f),
	m_color{}
{
}

TextUIGUI::~TextUIGUI()
{
}

void TextUIGUI::Update()
{
	if (false == Start())
		return;

	FONT_COLOR color;
	color.iColor = FONT_RGBA(m_color.r, m_color.g, m_color.b, m_color.a);

	tstring tStrContent{};
	StringToTString(m_strContent, tStrContent);
	CFontManager::GetInstance()->DrawFont(tStrContent.c_str(), m_vPos.x, m_vPos.y, m_fFontSize, color.iColor);
	End();
}