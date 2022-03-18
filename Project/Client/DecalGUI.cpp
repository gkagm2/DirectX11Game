#include "pch.h"
#include "DecalGUI.h"
#include "CImGuiManager.h"
#include "ParamGUI.h"
#include <Engine\CResourceManager.h>
#include <Engine\CDecal.h>

DecalGUI::DecalGUI() :
	ComponentGUI(E_ComponentType::Decal)
{
}

DecalGUI::~DecalGUI()
{
}

void DecalGUI::Update()
{
	if (false == Start())
		return;	

	ImGui::Text("Material");

	CTexture* pDecalTex = nullptr;
	if (nullptr != GetTargetObject()->Decal()->GetDecalTexture())
		pDecalTex = GetTargetObject()->Decal()->GetDecalTexture().Get();
	
	if (ParamGUI::Render_Texture("Select##DelcalTexture", pDecalTex, this, (GUI_CALLBACK)&DecalGUI::SelectTexture)) {
	}

	ImGui::PushID(CImGuiManager::GetInstance()->GetWidgetID());
	bool bLighting = GetTargetObject()->Decal()->IsLighting();
	if (ImGui::Checkbox("Lighting##Decal", &bLighting))
		GetTargetObject()->Decal()->SetLightToDecal(bLighting);
	ImGui::PopID();

	End();
}

void DecalGUI::SelectTexture(DWORD_PTR _pStr, DWORD_PTR _NONE)
{
	// 선택한 텍스쳐를 알아낸다.
	const char* pStr = (const char*)_pStr;
	string str = pStr;
	tstring tStr;
	StringToTString(str, tStr);

	CTexture* pTex = CResourceManager::GetInstance()->FindRes<CTexture>(tStr).Get();
	assert(pTex);

	if (pTex)
		GetTargetObject()->Decal()->SetDecalTexture(pTex);
}