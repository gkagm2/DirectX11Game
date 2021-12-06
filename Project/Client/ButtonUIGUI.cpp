#include "pch.h"
#include "ButtonUIGUI.h"
#include "ParamGUI.h"
#include <Engine\CButtonUI.h>
#include <Engine\CFontManager.h>

ButtonUIGUI::ButtonUIGUI() :
	ComponentGUI(E_ComponentType::ButtonUI)
{
}

ButtonUIGUI::~ButtonUIGUI()
{
}

void ButtonUIGUI::Update()
{
	if (false == Start())
		return;


	CButtonUI* pButton = GetTargetObject()->ButtonUI();
	UINT iNormalColor = pButton->GetNormalColor();
	UINT iHighlightedColor = pButton->GetHighlightedColor();
	UINT iPressedColor = pButton->GetPressedColor();
	UINT iSelectedColor = pButton->GetSelectedColor();
	UINT iDisableColor = pButton->GetDisableColor();
	float fFadeDuration = pButton->GetFadeDuration();

	char szBuff[255] = {};

	sprintf_s(szBuff, 255, "Normal Color##NormalColor");
	if (ParamGUI::Render_Color(szBuff, &iNormalColor))
		pButton->SetNormalColor(iNormalColor);

	sprintf_s(szBuff, 255, "Highlighted Color##HighlightedColor");
	if (ParamGUI::Render_Color(szBuff, &iHighlightedColor))
		pButton->SetHighlightedColor(iHighlightedColor);

	sprintf_s(szBuff, 255, "Pressed Color##PressedColor");
	if (ParamGUI::Render_Color(szBuff, &iPressedColor))
		pButton->SetPressedColor(iPressedColor);

	sprintf_s(szBuff, 255, "Selected Color##SelectedColor");
	if (ParamGUI::Render_Color(szBuff, &iSelectedColor))
		pButton->SetSelectedColor(iSelectedColor);

	sprintf_s(szBuff, 255, "Disable Color##DisableColor");
	if (ParamGUI::Render_Color(szBuff, &iDisableColor))
		pButton->SetDisableColor(iDisableColor);

	sprintf_s(szBuff, 255, "Fade Duration##FadeDuration");
	if (ParamGUI::Render_Float(szBuff, &fFadeDuration))
		pButton->SetFadeDuration(fFadeDuration);

	End();
}