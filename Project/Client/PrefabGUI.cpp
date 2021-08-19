#include "pch.h"
#include "PrefabGUI.h"
#include <Engine\CPrefab.h>
#include <Engine\CResourceManager.h>

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
		CObject::InstantiateEvn(pPrefab, m_iRespawnPos, (E_Layer)m_iLayer);
	}

	End();
}