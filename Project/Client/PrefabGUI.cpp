#include "pch.h"
#include "PrefabGUI.h"
#include <Engine\CPrefab.h>
#include <Engine\CResourceManager.h>

PrefabGUI::PrefabGUI() :
	ResourceGUI(E_ResourceType::Prefab)
{
}

PrefabGUI::~PrefabGUI()
{
}

void PrefabGUI::Update()
{
	if (false == Start())
		return;

	// InGame에 추가하기
	if (ImGui::Button("Add In game")) {
		CPrefab* pPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(GetTargetResource()->GetKey()).Get();
		CObject::InstantiateEvn(pPrefab, Vector3(0.f,0.f,0.f));
	}


	End();
}