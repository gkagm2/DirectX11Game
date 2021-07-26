#include "pch.h"
#include "InspectorGUI.h"

#include <Engine\CSceneManager.h>
#include <Engine\CScene.h>

#include "CImGuiManager.h"
#include "ComponentGUI.h"
#include "TransformGUI.h"
#include "MeshRendererGUI.h"
#include "Collider2DRectGUI.h"


InspectorGUI::InspectorGUI() :
	m_arrComGUI{},
	m_pTargetObject{nullptr}
{
	SetName("Inspector");
}

InspectorGUI::~InspectorGUI()
{
	Safe_Delete_Array(m_arrComGUI);
}

void InspectorGUI::Init()
{
	m_pTargetObject = CSceneManager::GetInstance()->FindGameObject(_T("Player"));

	// TransformGUI
	m_arrComGUI[(UINT)E_ComponentType::Transform] = new TransformGUI;
	m_arrComGUI[(UINT)E_ComponentType::Transform]->SetSize(ImVec2(0.f, 100.f));
	m_arrComGUI[(UINT)E_ComponentType::Transform]->SetTargetObject(m_pTargetObject);

	// MeshRendererGUI
	m_arrComGUI[(UINT)E_ComponentType::MeshRenderer] = new MeshRendererGUI;
	m_arrComGUI[(UINT)E_ComponentType::MeshRenderer]->SetSize(ImVec2(0.f, 100.f));
	m_arrComGUI[(UINT)E_ComponentType::MeshRenderer]->SetTargetObject(m_pTargetObject);

	m_arrComGUI[(UINT)E_ComponentType::Collider2D] = new Collider2DRectGUI;
	m_arrComGUI[(UINT)E_ComponentType::Collider2D]->SetSize(ImVec2(0.f, 100.f));
	m_arrComGUI[(UINT)E_ComponentType::Collider2D]->SetTargetObject(m_pTargetObject);
}

void InspectorGUI::Update()
{
	ImGui::Begin(GetName().c_str(), &m_bGUIOpen);

	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr == m_arrComGUI[i])
			continue;

		m_arrComGUI[i]->Update();
		ImGui::Separator();
	}

	ImGui::End();
}