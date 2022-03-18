#include "pch.h"
#include "ButtonUIGUI.h"
#include "ParamGUI.h"
#include <Engine\CButtonUI.h>
#include <Engine\CFontManager.h>
#include <Engine\CResourceManager.h>
#include "CImGuiManager.h"


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

	// 이미지 변경 버튼

	CTexture* pImageTexture = pButton->GetImageTex().Get();
	string strKey{};
	if (pImageTexture)
		TStringToString(pImageTexture->GetKey().c_str(), strKey);
		
	ParamGUI::Render_Texture(strKey.c_str(), pImageTexture, this, (GUI_CALLBACK)&ButtonUIGUI::_SelectHeightMapTexture, true);



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

void ButtonUIGUI::_SelectHeightMapTexture(DWORD_PTR _pStr, DWORD_PTR _NONE)
{
	const char* pStrKey = (const char*)_pStr;
	string strKey = pStrKey;
	tstring tStrKey;
	StringToTString(strKey, tStrKey);

	CTexture* pImageTexture = CResourceManager::GetInstance()->FindRes<CTexture>(tStrKey).Get();
	//assert(pImageTexture);
	GetTargetObject()->ButtonUI()->SetImageTex(pImageTexture);
}