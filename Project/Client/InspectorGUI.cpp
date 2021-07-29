#include "pch.h"
#include "InspectorGUI.h"

#include <Engine\CSceneManager.h>
#include <Engine\CScene.h>

#include "CImGuiManager.h"
#include "ComponentGUI.h"
#include "TransformGUI.h"
#include "MeshRendererGUI.h"
#include "Collider2DRectGUI.h"
#include "Rigidbody2DGUI.h"

#include "ResourceGUI.h"
#include "MaterialGUI.h"
#include "TextureGUI.h"

InspectorGUI::InspectorGUI() :
	m_arrComGUI{},
	m_pTargetObject{nullptr},
	m_arrResGUI{},
	m_pTargetResource{ nullptr },
	m_eMode(E_InspectorUIMode::None)
{
	SetName(STR_GUI_Inspector);
}

InspectorGUI::~InspectorGUI()
{
	Safe_Delete_Array(m_arrComGUI);
	Safe_Delete_Array(m_arrResGUI);
}

void InspectorGUI::Init()
{
	// Components

	// TransformGUI
	m_arrComGUI[(UINT)E_ComponentType::Transform] = new TransformGUI;
	m_arrComGUI[(UINT)E_ComponentType::Transform]->SetUISize(ImVec2(0.f, 110.f));
	m_arrComGUI[(UINT)E_ComponentType::Transform]->SetTargetObject(m_pTargetObject);

	// MeshRendererGUI
	m_arrComGUI[(UINT)E_ComponentType::MeshRenderer] = new MeshRendererGUI;
	m_arrComGUI[(UINT)E_ComponentType::MeshRenderer]->SetUISize(ImVec2(0.f, 110.f));
	m_arrComGUI[(UINT)E_ComponentType::MeshRenderer]->SetTargetObject(m_pTargetObject);

	m_arrComGUI[(UINT)E_ComponentType::Collider2D] = new Collider2DRectGUI;
	m_arrComGUI[(UINT)E_ComponentType::Collider2D]->SetUISize(ImVec2(0.f, 110.f));
	m_arrComGUI[(UINT)E_ComponentType::Collider2D]->SetTargetObject(m_pTargetObject);

	m_arrComGUI[(UINT)E_ComponentType::Rigidbody2D] = new Rigidbody2DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Rigidbody2D]->SetUISize(ImVec2(0.f, 110.f));
	m_arrComGUI[(UINT)E_ComponentType::Rigidbody2D]->SetTargetObject(m_pTargetObject);

	// Resources

	m_arrResGUI[(UINT)E_ResourceType::Material] = new MaterialGUI;
	m_arrResGUI[(UINT)E_ResourceType::Material]->SetUISize(ImVec2(0.f, 0.f));

	m_arrResGUI[(UINT)E_ResourceType::Texture] = new TextureGUI;
	m_arrResGUI[(UINT)E_ResourceType::Texture]->SetUISize(ImVec2(0.f, 0.f));
}

void InspectorGUI::Update()
{
	ImGui::Begin(GetName().c_str(), &m_bGUIOpen);

	switch (m_eMode) {
	case E_InspectorUIMode::GameObject:
		UpdateObjectGUI();
		break;
	case E_InspectorUIMode::Resource:
		UpdateResourceGUI();
		break;
	case E_InspectorUIMode::None:
		break;
	}

	ImGui::End();
}

void InspectorGUI::UpdateObjectGUI()
{
	ImGui::Separator();
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr == m_arrComGUI[i])
			continue;

		m_arrComGUI[i]->Update();
	}
}

void InspectorGUI::UpdateResourceGUI()
{
	ImGui::Separator();
	E_ResourceType eType =  m_pTargetResource->GetResourceType();
	if (nullptr == m_arrResGUI[(UINT)eType]) {
		assert(nullptr);
		return;
	}
	m_arrResGUI[(UINT)eType]->Update();
	ImGui::Separator();
}

void InspectorGUI::SetTargetObject(CGameObject* _pTargetObj)
{
	assert(_pTargetObj);
	m_eMode = E_InspectorUIMode::GameObject;
	m_pTargetObject = _pTargetObj;
	
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		// FIXED : TODO (Jang) : 모든 컴포넌트를 다 만들면 필요없음.
		if (nullptr == m_arrComGUI[i])
			continue;

		m_arrComGUI[i]->SetTargetObject(_pTargetObj);
	}
}

void InspectorGUI::SetTargetResource(CResource* _pTargetResource)
{
	assert(_pTargetResource);
	m_eMode = E_InspectorUIMode::Resource;
	m_pTargetResource = _pTargetResource;

	E_ResourceType eType = _pTargetResource->GetResourceType();

	if (nullptr == m_arrResGUI[(UINT)eType]) {
		assert(nullptr);
		return;
	}

	m_arrResGUI[(UINT)eType]->SetTargetResource(_pTargetResource);
}