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
#include "RectTransformGUI.h"
#include "CanvasRendererGUI.h"
#include "AudioSourceGUI.h"
#include "ParticleSystemGUI.h"

// Contents UI
#include "TextUIGUI.h"
#include "ButtonUIGUI.h"
#include "ImageUIGUI.h"

#include "ResourceGUI.h"
#include "MaterialGUI.h"
#include "TextureGUI.h"
#include "PrefabGUI.h"

#include "ScriptGUI.h"

#include <Script\CScriptMgr.h>
#include <Engine\CScript.h>


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
	Safe_Delete_Vector(m_vecScriptGUI);
}

void InspectorGUI::Init()
{
	///////////// Components

	// TransformGUI
	m_arrComGUI[(UINT)E_ComponentType::Transform] = new TransformGUI;
	m_arrComGUI[(UINT)E_ComponentType::Transform]->SetUISize(ImVec2(0.f, 110.f));

	// RectTransformGUI
	m_arrComGUI[(UINT)E_ComponentType::RectTransform] = new RectTransformGUI;
	m_arrComGUI[(UINT)E_ComponentType::RectTransform]->SetUISize(ImVec2(0.f, 150.f));

	// MeshRendererGUI
	m_arrComGUI[(UINT)E_ComponentType::MeshRenderer] = new MeshRendererGUI;
	m_arrComGUI[(UINT)E_ComponentType::MeshRenderer]->SetUISize(ImVec2(0.f, 110.f));

	// CanvasRendererGUI
	m_arrComGUI[(UINT)E_ComponentType::CanvasRenderer] = new CanvasRendererGUI;
	m_arrComGUI[(UINT)E_ComponentType::CanvasRenderer]->SetUISize(ImVec2(0.f, 20.f));

	// Collider2D
	m_arrComGUI[(UINT)E_ComponentType::Collider2D] = new Collider2DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Collider2D]->SetUISize(ImVec2(0.f, 110.f));

	// Rigidbody2D
	m_arrComGUI[(UINT)E_ComponentType::Rigidbody2D] = new Rigidbody2DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Rigidbody2D]->SetUISize(ImVec2(0.f, 220.f));

	// Animator2D
	m_arrComGUI[(UINT)E_ComponentType::Animator2D] = new Animator2DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Animator2D]->SetUISize(ImVec2(0.f, 510.f));

	// Light2D
	m_arrComGUI[(UINT)E_ComponentType::Light2D] = new Light2DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Light2D]->SetUISize(ImVec2(0.f, 0.f));

	// TileMap
	m_arrComGUI[(UINT)E_ComponentType::TileMap] = new TileMapGUI;
	m_arrComGUI[(UINT)E_ComponentType::TileMap]->SetUISize(ImVec2(0.f, 300.f));

	// AudioSource
	m_arrComGUI[(UINT)E_ComponentType::AudioSource] = new AudioSourceGUI;
	m_arrComGUI[(UINT)E_ComponentType::AudioSource]->SetUISize(ImVec2(0.f, 110.f));


	// CameraGUI
	m_arrComGUI[(UINT)E_ComponentType::Camera] = new CameraGUI;
	m_arrComGUI[(UINT)E_ComponentType::Camera]->SetUISize(ImVec2(0.f, 310.f));

	// ParticleSystemGUI
	m_arrComGUI[(UINT)E_ComponentType::ParticleSystem] = new ParticleSystemGUI;
	m_arrComGUI[(UINT)E_ComponentType::ParticleSystem]->SetUISize(ImVec2(0.f, 480.f));

	// UI
	// TextUI
	m_arrComGUI[(UINT)E_ComponentType::TextUI] = new TextUIGUI;
	m_arrComGUI[(UINT)E_ComponentType::TextUI]->SetUISize(ImVec2(0.f, 100.f));

	// ImageUI
	m_arrComGUI[(UINT)E_ComponentType::ImageUI] = new ImageUIGUI;
	m_arrComGUI[(UINT)E_ComponentType::ImageUI]->SetUISize(ImVec2(0.f, 0.f));

	// ButtonUI
	m_arrComGUI[(UINT)E_ComponentType::ButtonUI] = new ButtonUIGUI;
	m_arrComGUI[(UINT)E_ComponentType::ButtonUI]->SetUISize(ImVec2(0.f, 0.f));

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
	// TODO (Jang) : HierarchyGUI ���ο���  Focusing�� object. none���� �����ϰԲ� �ؾߵ�
	//if (CEventManager::GetInstance()->DidEventHappended())
	//	m_eMode = E_InspectorUIMode::None;

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
	if (nullptr == m_pTargetObject)
		return;
		
	if (m_pTargetObject->IsDead()) {
		m_pTargetObject = nullptr;
		return;
	}

	ImGui::BeginChild("Object view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
	ImGui::Separator();

	// �̸� �ٲٱ�
	ImGui::Text("Name"); ImGui::SameLine();
	char strObjName[255] = "";
	tstring strName = m_pTargetObject->GetName();
	TStringToArr(strName, strObjName, 255);
	if (ImGui::InputText("##GameObjectName", strObjName, 255)) {
		tstring tname;
		StringToTString(strObjName, tname);
		m_pTargetObject->SetName(tname);
		CObject::ChangeStateEvn();
	}

	// ����
	ImGui::SameLine();
	if (ImGui::Button("Del##DeleteGameObject")) {
		CObject::DestroyGameObjectEvn(m_pTargetObject);
		m_eMode = E_InspectorUIMode::None;
	}

	// ���������� �����
	if (ImGui::Button("Make Prefab##Make Prefab")) {
		m_pTargetObject->RegisterAsPrefab(m_pTargetObject->GetName());
	}

	// ����
	if (ImGui::Button("Clone##Clone GameObject")) {
		CGameObject* pCloneObj = m_pTargetObject->Clone();
		CObject::CreateGameObjectEvn(pCloneObj, m_pTargetObject->GetLayer());
		if (m_pTargetObject->GetParentObject())
			CObject::AddChildGameObjectEvn(m_pTargetObject->GetParentObject(), pCloneObj);
	}

	// Ȱ��ȭ  ����
	bool bActive = m_pTargetObject->IsActive();
	if (ImGui::Checkbox("Active##Set Active GameObject", &bActive)) {
		m_pTargetObject->SetActive(bActive);
		GUI::ChangeStateEvn(); // UI update
	}
	ImGui::SameLine();
	bool bActiveWithChilds = m_pTargetObject->IsActive();
	if (ImGui::Checkbox("Active(with child)##Set Active GameObject with childs", &bActiveWithChilds)) {
		m_pTargetObject->SetActive(bActiveWithChilds, true);
		GUI::ChangeStateEvn(); // UI update
	}

	ImGui::Separator();

	// ���̾� ����
	int iLayer = (int)m_pTargetObject->GetLayer();
	ImGui::Text("Cur Layer %d", iLayer);
	static int iSetLayer = -1;
	ImGui::InputInt("Set Layer##GameObject", &iSetLayer);
	iSetLayer = CMyMath::Clamp(iSetLayer, 0, MAX_SIZE_LAYER - 1);

	if (ImGui::Button("Sel##LayerSelect")) {
		if ((int)m_pTargetObject->GetLayer() != iSetLayer) {
			// ���̾ �ٲ۴�.
			// �ڽź��� �ڽı��� ��� �ٲٱ�
			CObject::ChangeLayerEvn(m_pTargetObject, iSetLayer);
		}
		iSetLayer = 0;
	}
	ImGui::Separator();

	// �±� ����
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

	// FIXED(Jang) : �׽�Ʈ��
	ImGui::Text("ID : [%s]", to_string(m_pTargetObject->GetUUID()).c_str());

	// ComponentGUI �����ֱ�
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr == m_arrComGUI[i])
			continue;
		m_arrComGUI[i]->Update();
	}

	// ��ũ��Ʈ GUI �����ֱ�
	const vector<CScript*>& vecScript = m_pTargetObject->GetScripts();
	_ResizeScriptGUICnt();
	for (int i = 0; i < (int)vecScript.size(); ++i) {
		ScriptGUI* pScriptGUI = m_vecScriptGUI[i];
		pScriptGUI->SetTargetObject(m_pTargetObject);
		pScriptGUI->SetScript(vecScript[i]);
		pScriptGUI->Update();
	}

	ImGui::EndChild();

	// ������Ʈ �߰� ��ư
	if (ImGui::Button("Add Component##ComponentAdd")) {
		// ����Ʈ �並 �����ش�.
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

	// ��ũ��Ʈ �߰� ��ư
	if (ImGui::Button("Add Script##ScriptAdd")) {
		// ����Ʈ �並 �����ش�.
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
	if (nullptr == m_pTargetResource)
		return;
	ImGui::Separator();

	// �̸� �ٲٱ�
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
	
	// ������Ʈ GUI ����
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		// FIXED : TODO (Jang) : ��� ������Ʈ�� �� ����� �ʿ����.
		if (nullptr == m_arrComGUI[i])
			continue;

		m_arrComGUI[i]->SetTargetObject(_pTargetObj);
	}

	// ��ũ��Ʈ GUI ����
	const vector<CScript*>& vecScript = m_pTargetObject->GetScripts();	
	_ResizeScriptGUICnt();
	for (int i = 0; i < (int)vecScript.size(); ++i) {
		ScriptGUI* pScriptGUI = m_vecScriptGUI[i];
		pScriptGUI->SetTargetObject(m_pTargetObject);
		pScriptGUI->SetScript(vecScript[i]);
	}
	m_pTargetResource = nullptr;
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
	m_pTargetObject = nullptr;
}

void InspectorGUI::_AddNewComponent(DWORD_PTR _dw1, DWORD_PTR _dw2)
{
	string strComponent = (char*)_dw1;
	tstring tstrComponent;
	StringToTString(strComponent, tstrComponent);
	UINT i = ComponentStrToIdx(tstrComponent);
	CComponent* pNewComponent = m_pTargetObject->CreateComponent((E_ComponentType)i);
	m_pTargetObject->AddComponent(pNewComponent);
	// FIXED (Jang) : �̺�Ʈ�� ���ص� �Ƿ���?
}

void InspectorGUI::_AddNewScriptComponent(DWORD_PTR _dw1, DWORD_PTR _dw2)
{
	string strComponent = (char*)_dw1;
	tstring tstrComponent;
	StringToTString(strComponent, tstrComponent);
	CComponent* pScriptComponent = (CComponent*)CScriptMgr::GetScript(tstrComponent);
	m_pTargetObject->AddComponent(pScriptComponent);
}

void InspectorGUI::_ResizeScriptGUICnt()
{
	const vector<CScript*>& vecScript = m_pTargetObject->GetScripts();
	size_t iSize = vecScript.size();
	size_t iGUISize = m_vecScriptGUI.size();
	if (iSize > iGUISize) {
		size_t iNeedCnt = iSize -iGUISize;
		for (size_t i = 0; i < iNeedCnt; ++i)
			m_vecScriptGUI.push_back(new ScriptGUI);
	}
}
