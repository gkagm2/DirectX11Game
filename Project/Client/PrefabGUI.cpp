#include "pch.h"
#include "PrefabGUI.h"
#include <Engine\CPrefab.h>
#include <Engine\CResourceManager.h>
#include "CImGuiManager.h"
#include "InspectorGUI.h"

PrefabGUI::PrefabGUI() :
	ResourceGUI(E_ResourceType::Prefab),
	m_iLayer(0)
{
}

PrefabGUI::~PrefabGUI()
{
}

void PrefabGUI::Update()
{
	if (false == Start())
		return;

	CPrefab* pPrefab = (CPrefab*)GetTargetResource();

	// 이름 바꾸기
	ImGui::Text("Prefab Name"); ImGui::SameLine();
	static char strResourceName[255] = "";
	ImGui::InputText("##PrefName", strResourceName, 255);
	ImGui::SameLine();

	if (ImGui::Button("Change##PrefabName")) {
		if (strcmp(strResourceName, "") != 0) {
			tstring strKey;
			StringToTString(strResourceName, strKey);
			CResourceManager::GetInstance()->ChangeResourceKeyEvn(GetTargetResource()->GetKey(), strKey, GetTargetResource()->GetResourceType());
			memset(strResourceName, 0, 255);
			/*InspectorGUI* pInspectorGUI = dynamic_cast<InspectorGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_Inspector));
			if (pInspectorGUI)
				pInspectorGUI->SetInspectorUIMode(E_InspectorUIMode::None);*/
		}
	}
	static char strObjectName[255] = "";
	ImGui::Text("Object Name"); ImGui::SameLine();
	ImGui::InputText("##ObjName", strObjectName, 255);
	ImGui::SameLine();
	if (ImGui::Button("Change##ObjectNameInPrefab")) {
		if (strcmp(strObjectName, "") != 0) {
			tstring strName;
			StringToTString(strObjectName, strName);
			pPrefab->GetProtoObj()->SetName(strName);
			tstring relativepath = pPrefab->GetRelativePath();
			pPrefab->Save(pPrefab->GetRelativePath());
			memset(strObjectName, 0, 255);
		}
	}


	// 삭제
	if (ImGui::Button("Del##PrefabDelete")) {
		CResourceManager::GetInstance()->DeleteCustomResourceEvn(pPrefab->GetKey(), pPrefab->GetResourceType());

		tstring strFilePath = CPathManager::GetInstance()->GetContentPath();
		strFilePath += pPrefab->GetRelativePath();
		_tremove(strFilePath.c_str());
		InspectorGUI* pInspectorGUI = (InspectorGUI * )CImGuiManager::GetInstance()->FindGUI(STR_GUI_Inspector);
		pInspectorGUI->SetInspectorUIMode(E_InspectorUIMode::None);
	}

	if (pPrefab) {
		tstring tstrName = pPrefab->GetProtoObj()->GetName();
		string strName;
		TStringToString(tstrName, strName);
		ImGui::Text("Game Object Name : %s", strName.c_str());
	}
	else {
		ImGui::Text("Doesn't Exist GameObject");
	}

	// 들어갈 레이어
	string strLayerName = "NONE";
	if (m_iLayer > 0) {
		m_iLayer = min(MAX_SIZE_LAYER - 1, m_iLayer);
		tstring tStrLayerName = LayerToString((E_Layer)m_iLayer);
		TStringToString(tStrLayerName, strLayerName);
	}
	ImGui::Text("Insert Layer [%s] [%d]", strLayerName.c_str(), m_iLayer);
	ImGui::DragInt("##PrefabGUI Insert Layer", &m_iLayer, 1, 0, MAX_SIZE_LAYER - 1, "%d");
	ImGui::InputFloat3("Respawn Position", (float*)&m_iRespawnPos, "%.2f");

	// InGame에 추가하기
	if (ImGui::Button("Add In game")) {
		CPrefab* pPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(GetTargetResource()->GetKey()).Get();
		CObject::InstantiateEvn(pPrefab, m_iRespawnPos, m_iLayer);
	}

	End();
}