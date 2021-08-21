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

	if (ImGui::Button("Del##PrefabDelete")) {
		CResourceManager::GetInstance()->DeleteCustomResourceEvn(pPrefab->GetKey(), pPrefab->GetResourceType());

		tstring strFilePath = CPathManager::GetInstance()->GetContentPath();
		strFilePath += pPrefab->GetRelativePath();
		_tremove(strFilePath.c_str());
		InspectorGUI* pInspectorGUI = (InspectorGUI * )CImGuiManager::GetInstance()->FindGUI(STR_GUI_Inspector);
		pInspectorGUI->SetInspectorUIMode(E_InspectorUIMode::None);
	}

	tstring tstrName = pPrefab->GetProtoObj()->GetName();
	string strName;
	TStringToString(tstrName, strName);
	ImGui::Text("Name : %s", strName.c_str());

	// 들어갈 레이어
	ImGui::DragInt("Insert Layer", &m_iLayer, 1, 0, MAX_SIZE_LAYER - 1, "%d");
	ImGui::InputFloat3("Respawn Position", (float*)&m_iRespawnPos, "%.2f");

	// InGame에 추가하기
	if (ImGui::Button("Add In game")) {
		CPrefab* pPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(GetTargetResource()->GetKey()).Get();
		CObject::InstantiateEvn(pPrefab, m_iRespawnPos, m_iLayer);
	}

	End();
}