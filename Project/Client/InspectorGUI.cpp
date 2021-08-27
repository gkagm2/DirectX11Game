#include "pch.h"
#include "InspectorGUI.h"

#include <Engine\CSceneManager.h>
#include <Engine\CScene.h>

#include "ListViewGUI.h"
#include <Engine\CResourceManager.h>
#include <Engine\CMeshRenderer.h>
#include <Engine\CPrefab.h>
#include <Engine\CEventManager.h>

#include "CImGuiManager.h"
#include "ComponentGUI.h"
#include "TransformGUI.h"
#include "CameraGUI.h"
#include "MeshRendererGUI.h"
#include "Rigidbody2DGUI.h"
#include "Animator2DGUI.h"
#include "Light2DGUI.h"
#include "TileMapGUI.h"
#include "Collider2DGUI.h"

#include "ResourceGUI.h"
#include "MaterialGUI.h"
#include "TextureGUI.h"
#include "PrefabGUI.h"

#include "ScriptGUI.h"

#include <Script\CScriptMgr.h>


InspectorGUI::InspectorGUI() :
	m_arrComGUI{},
	m_pScriptGUI{},
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
	if (m_pScriptGUI)
		delete m_pScriptGUI;
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
	m_arrComGUI[(UINT)E_ComponentType::Collider2D] = new Collider2DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Collider2D]->SetUISize(ImVec2(0.f, 110.f));

	// Rigidbody2D
	m_arrComGUI[(UINT)E_ComponentType::Rigidbody2D] = new Rigidbody2DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Rigidbody2D]->SetUISize(ImVec2(0.f, 160.f));

	// Animator2D
	m_arrComGUI[(UINT)E_ComponentType::Animator2D] = new Animator2DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Animator2D]->SetUISize(ImVec2(0.f, 110.f));

	// Light2D
	m_arrComGUI[(UINT)E_ComponentType::Light2D] = new Light2DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Light2D]->SetUISize(ImVec2(0.f, 0.f));

	// TileMap
	m_arrComGUI[(UINT)E_ComponentType::TileMap] = new TileMapGUI;
	m_arrComGUI[(UINT)E_ComponentType::TileMap]->SetUISize(ImVec2(0.f, 0.f));

	// CameraGUI
	m_arrComGUI[(UINT)E_ComponentType::Camera] = new CameraGUI;
	m_arrComGUI[(UINT)E_ComponentType::Camera]->SetUISize(ImVec2(0.f, 310.f));

	// Script GUI
	m_pScriptGUI = new ScriptGUI;
	m_pScriptGUI->SetUISize(ImVec2(0.f, 150.f));

	//////////// Resources

	m_arrResGUI[(UINT)E_ResourceType::Material] = new MaterialGUI;
	m_arrResGUI[(UINT)E_ResourceType::Material]->SetUISize(ImVec2(0.f, 0.f));

	m_arrResGUI[(UINT)E_ResourceType::Texture] = new TextureGUI;
	m_arrResGUI[(UINT)E_ResourceType::Texture]->SetUISize(ImVec2(0.f, 0.f));

	m_arrResGUI[(UINT)E_ResourceType::Prefab] = new PrefabGUI;
	m_arrResGUI[(UINT)E_ResourceType::Prefab]->SetUISize(ImVec2(0.f, 0.f));
}

void InspectorGUI::Update()
{
	/*if (CEventManager::GetInstance()->DidEventHappended())
		m_eMode = E_InspectorUIMode::None;*/

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
	ImGui::BeginChild("Object view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
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

	// 프리펩으로 만들기
	if (ImGui::Button("Make Prefab##Make Prefab")) {
		tstring strPrefabName = STR_FILE_PATH_Prefab + m_pTargetObject->GetName() + _T(".pref");
		m_pTargetObject->RegisterAsPrefab(strPrefabName);
		SharedPtr<CPrefab> pPrefab = CResourceManager::GetInstance()->FindRes<CPrefab>(strPrefabName);
		pPrefab->SetRelativePath(strPrefabName);
		tstring strRelativePath = strPrefabName;
		pPrefab->Save(strRelativePath);
	}

	// 복사
	if (ImGui::Button("Clone##Clone GameObject")) {
		CGameObject* pCloneObj = m_pTargetObject->Clone();
		CObject::CreateGameObjectEvn(pCloneObj, m_pTargetObject->GetLayer());
	}

	ImGui::Separator();

	// 레이어 변경
	int iLayer = (int)m_pTargetObject->GetLayer();
	ImGui::Text("Cur Layer %d", iLayer);
	static int iSetLayer = -1;
	ImGui::InputInt("Set Layer##GameObject", &iSetLayer);
	iSetLayer = CMyMath::Clamp(iSetLayer, 0, MAX_SIZE_LAYER - 1);

	if (ImGui::Button("Sel##LayerSelect")) {
		if ((int)m_pTargetObject->GetLayer() != iSetLayer) {
			// 레이어를 바꾼다.
			// 자신부터 자식까지 모두 바꾸기
			CObject::ChangeLayerEvn(m_pTargetObject, iSetLayer);
		}
		iSetLayer = 0;
	}
	ImGui::Separator();

	// 태그 변경
	int iTag = (int)m_pTargetObject->GetTag();
	ImGui::Text("Cur Tag %d", iTag);
	static int iSetTag = 0;
	ImGui::InputInt("Set Tag##GameObject", &iSetTag);
	iSetTag = CMyMath::Clamp(iSetTag, 0, MAX_SIZE_TAG - 1);
	if (ImGui::Button("Sel##TagSelect")) {
		if ((int)m_pTargetObject->GetTag() != iSetTag)
			m_pTargetObject->SetTag((UINT)iSetTag);
		iSetTag = 0;
	}


	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr == m_arrComGUI[i])
			continue;

		m_arrComGUI[i]->Update();
	}
	if (m_pScriptGUI)
		m_pScriptGUI->Update();

	ImGui::EndChild();


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

	ImGui::SameLine();

	// 스크립트 추가 버튼
	if (ImGui::Button("Add Script##ScriptAdd")) {
		// 리스트 뷰를 보여준다.
		ListViewGUI* pListViewGUI = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
		assert(pListViewGUI);

		vector<tstring> vecScriptNames;
		CScriptMgr::GetScriptInfo(vecScriptNames);
		
		pListViewGUI->SetList(vecScriptNames, _T("Script"));
		pListViewGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&InspectorGUI::_AddNewScriptComponent);
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

	if (nullptr == m_arrResGUI[(UINT)eType]) {
		return;
	}
	m_arrResGUI[(UINT)eType]->Update();
	ImGui::Separator();
}

void InspectorGUI::SetTargetObject(CGameObject* _pTargetObj)
{
	if (nullptr == _pTargetObj)
		return;

	m_eMode = E_InspectorUIMode::GameObject;
	m_pTargetObject = _pTargetObj;
	
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		// FIXED : TODO (Jang) : 모든 컴포넌트를 다 만들면 필요없음.
		if (nullptr == m_arrComGUI[i])
			continue;

		m_arrComGUI[i]->SetTargetObject(_pTargetObj);
	}
	if (m_pScriptGUI)
		m_pScriptGUI->SetTargetObject(_pTargetObj);
}

void InspectorGUI::SetTargetResource(CResource* _pTargetResource)
{
	if (nullptr == _pTargetResource)
		return;
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

void InspectorGUI::_AddNewScriptComponent(DWORD_PTR _dw1, DWORD_PTR _dw2)
{
	string strComponent = (char*)_dw1;
	tstring tstrComponent;
	StringToTString(strComponent, tstrComponent);
	CComponent* pScriptComponent = (CComponent*)CScriptMgr::GetScript(tstrComponent);
	m_pTargetObject->AddComponent(pScriptComponent);
}
