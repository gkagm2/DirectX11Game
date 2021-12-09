#include "pch.h"
#include "ScriptGUI.h"
#include <Engine\CGameObject.h>
#include <Script\CScriptMgr.h>
#include <Engine\CScript.h>

ScriptGUI::ScriptGUI() :
	ComponentGUI(E_ComponentType::Script)
{
}

ScriptGUI::~ScriptGUI()
{
}
bool ScriptGUI::Start()
{
	if (nullptr == GetTargetObject())
		return false;

	if (GetTargetObject()->IsDead()) {
		SetTargetObject(nullptr);
		return false;
	}

	const vector<CScript*>& vecScripts = GetTargetObject()->GetScripts();
	if (vecScripts.empty())
		return false;

	if (!IsActive())
		return false;

	ImGui::BeginChild(GetName().c_str(), GetUISize());

	string strComponentName;
	WStringToString(ComponentTypeToStr(GetComponentType()), strComponentName);
	SetTitileButton((int)GetComponentType(), strComponentName);

	//ImGui::SameLine();

	// Delete Component Button
	/*if (ImGui::Button("Del")) {
		CComponent* pComponent = m_pTargetTileMap->GetComponent(m_eComponentType);
		CObject::DestroyObjectEvn(pComponent);
	}*/

	return true;
}
void ScriptGUI::Update()
{
	if (false == Start())
		return;

	CGameObject* pTargetObj = GetTargetObject();
	const vector<CScript*>& vecScripts = pTargetObj->GetScripts();
	
	for (UINT i = 0; i < vecScripts.size(); ++i) {
		
		tstring tstrScriptName = CScriptMgr::GetScriptName(vecScripts[i]);
		string strScriptName;
		TStringToString(tstrScriptName, strScriptName);
		string str = "Del##ScriptDelete" +  std::to_string(i);
		if (ImGui::Button(str.c_str())) {
			// Script »èÁ¦
			CScript* pScript = vecScripts[i];
			CObject::DestroyScriptEvn(pTargetObj, pScript);
		}
		ImGui::SameLine();
		ImGui::Text("Name : "); ImGui::SameLine();
		ImGui::Text(strScriptName.c_str());
		
	}

	End();
}