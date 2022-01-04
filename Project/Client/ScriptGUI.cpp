#include "pch.h"
#include "ScriptGUI.h"
#include <Engine\CGameObject.h>
#include <Script\CScriptMgr.h>
#include <Engine\CScript.h>
#include "CImGuiManager.h"
#include "ParamGUI.h"

ScriptGUI::ScriptGUI() :
	ComponentGUI(E_ComponentType::Script),
	m_pScript(nullptr),
	m_fHeight(10.f)
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

	if (!IsActive())
		return false;

	if (!m_pScript)
		return false;

	tstring tstrScriptName = CScriptMgr::GetScriptName(m_pScript);
	string strScriptName;
	TStringToString(tstrScriptName, strScriptName);
	SetName(strScriptName);

	SetUISize(ImVec2(0.f, m_fHeight));
	ImGui::BeginChild(GetName().c_str(), GetUISize());
	m_fHeight = 0.f;

	UINT iWidgetId = CImGuiManager::GetInstance()->GetWidgetID();
	string name = GetName();
	SetTitileButton(iWidgetId, GetName());

	ImGui::SameLine();

	// Delete Button
	UINT iWidgetID = CImGuiManager::GetInstance()->GetWidgetID();
	ImGui::PushID(iWidgetID);
	string str = "Del##ScriptDelete" + std::to_string(iWidgetID);
	if (ImGui::Button(str.c_str())) {
		CGameObject* pTargetObj = GetTargetObject();
		CObject::DestroyScriptEvn(pTargetObj, m_pScript);
	}
	ImGui::PopID();
	m_fHeight += ImGui::GetItemRectSize().y;

	return true;
}
void ScriptGUI::Update()
{
	if (false == Start())
		return;

	// �Է� ������ ���
	vector<TScriptParam>& vecParams = m_pScript->GetParams();
	for (size_t i = 0; i < vecParams.size(); ++i) {
		float fHeight = 0.f;
		string strParamName;
		TStringToString(vecParams[i].strName, strParamName);
		
		switch (vecParams[i].eParam) {
		case E_ScriptParam::INT:
			ParamGUI::Render_Int(strParamName, (int*)vecParams[i].pData, &fHeight);
			break;
		case E_ScriptParam::BOOL: {
			ImGui::Checkbox(strParamName.c_str(), (bool*)vecParams[i].pData);
			fHeight += ImGui::GetItemRectSize().y;
		}
			break;
		case E_ScriptParam::FLOAT:
			ParamGUI::Render_Float(strParamName, (float*)vecParams[i].pData, &fHeight);
			break;
		case E_ScriptParam::GAMEOBJ: {
			CGameObject** ppObj = (CGameObject**)(vecParams[i].ppData);
			ParamGUI::Render_GameObjectLink(strParamName, ppObj, &fHeight);
		}
			break;
		case E_ScriptParam::PREFAB:
			assert(nullptr);
			break;
		case E_ScriptParam::TEX:
			assert(nullptr);
			break;
		case E_ScriptParam::VEC2: {
			Vector2* vData = (Vector2*)vecParams[i].pData;
				ParamGUI::Render_Vector2(strParamName, vData, &fHeight);
				vecParams[i].pData = vData;
		}
			break;
		case E_ScriptParam::VEC3:{
			Vector3* vData = (Vector3*)vecParams[i].pData;
			ParamGUI::Render_Vector3(strParamName, vData, &fHeight);
			vecParams[i].pData = vData;
		}
			break;
		case E_ScriptParam::VEC4: {
			Vector4* vData = (Vector4*)vecParams[i].pData;
			ParamGUI::Render_Vector4(strParamName, vData, &fHeight);
			vecParams[i].pData = vData;
		}
			break;
		default:
			assert(nullptr && _T("��ũ��Ʈ �Ķ���Ͱ� ��ġ���� ����"));
			break;
		}
		m_fHeight += fHeight;
	}
	m_fHeight += ImGui::GetItemRectSize().y;

	End();
}