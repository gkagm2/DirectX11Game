#include "pch.h"
#include "InspectorGUI.h"

#include <Engine\CSceneManager.h>
#include <Engine\CScene.h>

#include "ListViewGUI.h"
#include <Engine\CResourceManager.h>
#include <Engine\CMeshRenderer.h>

#include "CImGuiManager.h"
#include "ComponentGUI.h"
#include "TransformGUI.h"
#include "CameraGUI.h"
#include "MeshRendererGUI.h"
#include "Collider2DRectGUI.h"
#include "Rigidbody2DGUI.h"
#include "Animator2DGUI.h"
#include "Light2DGUI.h"
#include "TileMapGUI.h"

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
	///////////// Components

	// TransformGUI
	m_arrComGUI[(UINT)E_ComponentType::Transform] = new TransformGUI;
	m_arrComGUI[(UINT)E_ComponentType::Transform]->SetUISize(ImVec2(0.f, 110.f));

	// MeshRendererGUI
	m_arrComGUI[(UINT)E_ComponentType::MeshRenderer] = new MeshRendererGUI;
	m_arrComGUI[(UINT)E_ComponentType::MeshRenderer]->SetUISize(ImVec2(0.f, 110.f));

	// Collider2D
	m_arrComGUI[(UINT)E_ComponentType::Collider2D] = new Collider2DRectGUI;
	m_arrComGUI[(UINT)E_ComponentType::Collider2D]->SetUISize(ImVec2(0.f, 110.f));

	// Rigidbody2D
	m_arrComGUI[(UINT)E_ComponentType::Rigidbody2D] = new Rigidbody2DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Rigidbody2D]->SetUISize(ImVec2(0.f, 160.f));

	// Animator2D
	m_arrComGUI[(UINT)E_ComponentType::Animator2D] = new Animator2DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Animator2D]->SetUISize(ImVec2(0.f, 110.f));

	// Light2D
	m_arrComGUI[(UINT)E_ComponentType::Light2D] = new Light2DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Light2D]->SetUISize(ImVec2(0.f, 110.f));

	// TileMap
	m_arrComGUI[(UINT)E_ComponentType::TileMap] = new TileMapGUI;
	m_arrComGUI[(UINT)E_ComponentType::TileMap]->SetUISize(ImVec2(0.f, 110.f));

	// CameraGUI
	m_arrComGUI[(UINT)E_ComponentType::Camera] = new CameraGUI;
	m_arrComGUI[(UINT)E_ComponentType::Camera]->SetUISize(ImVec2(0.f, 310.f));

	//////////// Resources

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
	default:
		assert(nullptr);
		break;
	}

	ImGui::End();
}

void InspectorGUI::UpdateObjectGUI()
{
	ImGui::Separator();
	
	// 이름 바꾸기
	ImGui::Text("Name"); ImGui::SameLine();
	char strObjName[255] = "";
	TStringToArr(m_pTargetObject->GetName(), strObjName, 255);
	if (ImGui::InputText("##GameObjectName", strObjName, 255)) {
		tstring tname;
		StringToTString(strObjName, tname);
		m_pTargetObject->SetName(tname);
		CObject::ChangeStateEvn();
	}

	// 삭제
	ImGui::SameLine();
	if (ImGui::Button("Del##DeleteGameObject")) {
		CObject::DestroyGameObjectEvn(m_pTargetObject);
		m_eMode = E_InspectorUIMode::None;
	}

	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr == m_arrComGUI[i])
			continue;

		m_arrComGUI[i]->Update();
	}

	// 컴포넌트 추가 버튼
	if (ImGui::Button("Add Component##ComponentAdd")) {
		// 리스트 뷰를 보여준다.
		ListViewGUI* pListViewGUI = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
		assert(pListViewGUI);

		vector<tstring> vecNames;
		for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
			CComponent* pComponent = m_pTargetObject->GetComponent((E_ComponentType)i);
			if (pComponent)
				continue;
			tstring strComponentName = ComponentTypeToStr((E_ComponentType)i);
			vecNames.push_back(strComponentName);
		}

		pListViewGUI->SetList(vecNames, _T("Component"));
		pListViewGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&InspectorGUI::_AddNewComponent);
		pListViewGUI->SetActive(true);
	}
}

void InspectorGUI::UpdateResourceGUI()
{
	ImGui::Separator();

	// 이름 바꾸기
	ImGui::Text("Name"); ImGui::SameLine();
	char strObjName[255] = "";
	TStringToArr(m_pTargetResource->GetName(), strObjName, 255);

	if (ImGui::InputText("##ResourceName", strObjName, 255)) {
		tstring tname;
		StringToTString(strObjName, tname);
		m_pTargetResource->SetName(tname);
		CObject::ChangeStateEvn();
	}

	E_ResourceType eType = m_pTargetResource->GetResourceType();

	if (eType == E_ResourceType::Material) {
		// 메터리얼 삭제
		CMaterial* pMtrl = (CMaterial*)m_pTargetResource;
		if (false == pMtrl->IsDefaultMaterial()) {
			// Delete Resource Button
			ImGui::SameLine();
			if (ImGui::Button("Del##Resource")) {
				// Resource 삭제
				CResourceManager::GetInstance()->DeleteCopiedMaterialEvn(pMtrl->GetKey());
				m_eMode = E_InspectorUIMode::None;
			}
		}
	}

	if (nullptr == m_arrResGUI[(UINT)eType]) {
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
		return;
	}

	m_arrResGUI[(UINT)eType]->SetTargetResource(_pTargetResource);
}

void InspectorGUI::_AddNewComponent(DWORD_PTR _dw1, DWORD_PTR _dw2)
{
	string strComponent = (char*)_dw1;
	tstring tstrComponent;
	StringToTString(strComponent, tstrComponent);
	UINT i = ComponentStrToIdx(tstrComponent);
	CComponent* pNewComponent = m_pTargetObject->CreateComponent((E_ComponentType)i);
	m_pTargetObject->AddComponent(pNewComponent);
	// FIXED (Jang) : 이벤트로 안해도 되려나?
}