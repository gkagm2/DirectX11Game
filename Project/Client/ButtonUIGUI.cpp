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
	ImGui::Text("Texture ");
	if (ImGui::Button("Change")) {
		ListViewGUI* pListView = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
		assert(pListView);
		if (pListView) {
			// Texture Names
			vector<tstring> vecNames;
			CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Texture, vecNames);
			pListView->SetList(vecNames, ResourceTypeToStr(E_ResourceType::Texture));
			pListView->SetDoubleClickCallBack(this, (GUI_CALLBACK)&ButtonUIGUI::SelectTexture);
			pListView->SetActive(true);
		}
	}

	CTexture* pImageTexture = pButton->GetImageTex().Get();
	if (pImageTexture) {
		string strKey;
		TStringToString(pImageTexture->GetKey().c_str(), strKey);
		if (ParamGUI::Render_Texture(strKey.c_str(), pImageTexture, nullptr, nullptr, false)) {
		}
	}



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

void ButtonUIGUI::SelectTexture(DWORD_PTR _pStr, DWORD_PTR _NONE)
{
	const char* pStrKey = (const char*)_pStr;
	string strKey = pStrKey;
	tstring tStrKey;
	StringToTString(strKey, tStrKey);

	CTexture* pImageTexture = CResourceManager::GetInstance()->FindRes<CTexture>(tStrKey).Get();
	assert(pImageTexture);
	GetTargetObject()->ButtonUI()->SetImageTex(pImageTexture);
}