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
#include "Animator3DGUI.h"
#include "Light2DGUI.h"
#include "Light3DGUI.h"
#include "TileMapGUI.h"
#include "Collider2DGUI.h"
#include "Collider3DGUI.h"
#include "RectTransformGUI.h"
#include "CanvasRendererGUI.h"
#include "AudioSourceGUI.h"
#include "ParticleSystemGUI.h"
#include "SkyboxGUI.h"
#include "DecalGUI.h"
#include "BoundingBoxGUI.h"
#include "TerrainGUI.h"

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

	// BoundingBoxGUI
	m_arrComGUI[(UINT)E_ComponentType::BoundingBox] = new BoundingBoxGUI;
	m_arrComGUI[(UINT)E_ComponentType::BoundingBox]->SetUISize(ImVec2(0.f, 0.f));

	// MeshRendererGUI
	m_arrComGUI[(UINT)E_ComponentType::MeshRenderer] = new MeshRendererGUI;
	m_arrComGUI[(UINT)E_ComponentType::MeshRenderer]->SetUISize(ImVec2(0.f, 110.f));

	// CanvasRendererGUI
	m_arrComGUI[(UINT)E_ComponentType::CanvasRenderer] = new CanvasRendererGUI;
	m_arrComGUI[(UINT)E_ComponentType::CanvasRenderer]->SetUISize(ImVec2(0.f, 30.f));

	// Collider2D
	m_arrComGUI[(UINT)E_ComponentType::Collider2D] = new Collider2DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Collider2D]->SetUISize(ImVec2(0.f, 110.f));

	// Collider3D
	m_arrComGUI[(UINT)E_ComponentType::Collider3D] = new Collider3DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Collider3D]->SetUISize(ImVec2(0.f, 0.f));

	// Rigidbody2D
	m_arrComGUI[(UINT)E_ComponentType::Rigidbody2D] = new Rigidbody2DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Rigidbody2D]->SetUISize(ImVec2(0.f, 220.f));

	// Animator2D
	m_arrComGUI[(UINT)E_ComponentType::Animator2D] = new Animator2DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Animator2D]->SetUISize(ImVec2(0.f, 370.f));

	// Animator3D
	m_arrComGUI[(UINT)E_ComponentType::Animator3D] = new Animator3DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Animator3D]->SetUISize(ImVec2(0.f, 0.f));

	// Light2D
	m_arrComGUI[(UINT)E_ComponentType::Light2D] = new Light2DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Light2D]->SetUISize(ImVec2(0.f, 200.f));

	// Light3D
	m_arrComGUI[(UINT)E_ComponentType::Light3D] = new Light3DGUI;
	m_arrComGUI[(UINT)E_ComponentType::Light3D]->SetUISize(ImVec2(0.f, 200.f));

	// TileMap
	m_arrComGUI[(UINT)E_ComponentType::TileMap] = new TileMapGUI;
	m_arrComGUI[(UINT)E_ComponentType::TileMap]->SetUISize(ImVec2(0.f, 300.f));

	// AudioSource
	m_arrComGUI[(UINT)E_ComponentType::AudioSource] = new AudioSourceGUI;
	m_arrComGUI[(UINT)E_ComponentType::AudioSource]->SetUISize(ImVec2(0.f, 110.f));


	// CameraGUI
	m_arrComGUI[(UINT)E_ComponentType::Camera] = new CameraGUI;
	m_arrComGUI[(UINT)E_ComponentType::Camera]->SetUISize(ImVec2(0.f, 330.f));

	// ParticleSystemGUI
	m_arrComGUI[(UINT)E_ComponentType::ParticleSystem] = new ParticleSystemGUI;
	m_arrComGUI[(UINT)E_ComponentType::ParticleSystem]->SetUISize(ImVec2(0.f, 500.f));

	// SkyboxGUI
	m_arrComGUI[(UINT)E_ComponentType::Skybox] = new SkyboxGUI;
	m_arrComGUI[(UINT)E_ComponentType::Skybox]->SetUISize(ImVec2(0.f, 0.f));

	// DecalGUI
	m_arrComGUI[(UINT)E_ComponentType::Decal] = new DecalGUI;
	m_arrComGUI[(UINT)E_ComponentType::Decal]->SetUISize(ImVec2(0.f, 300.f));

	// Terrain
	m_arrComGUI[(UINT)E_ComponentType::Terrain] = new TerrainGUI;
	m_arrComGUI[(UINT)E_ComponentType::Terrain]->SetUISize(ImVec2(0.f, 3000.f));

	// UI
	// TextUI
	m_arrComGUI[(UINT)E_ComponentType::TextUI] = new TextUIGUI;
	m_arrComGUI[(UINT)E_ComponentType::TextUI]->SetUISize(ImVec2(0.f, 220.f));

	// ImageUI
	m_arrComGUI[(UINT)E_ComponentType::ImageUI] = new ImageUIGUI;
	m_arrComGUI[(UINT)E_ComponentType::ImageUI]->SetUISize(ImVec2(0.f, 350.f));

	// ButtonUI
	m_arrComGUI[(UINT)E_ComponentType::ButtonUI] = new ButtonUIGUI;
	m_arrComGUI[(UINT)E_ComponentType::ButtonUI]->SetUISize(ImVec2(0.f, 450.f));

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
	// TODO (Jang) : HierarchyGUI 내부에서  Focusing된 object. none으로 설정하게끔 해야됨
	//if (CEventManager::GetInstance()->DidEventHappended())
	//	m_eMode = E_InspectorUIMode::None;
	if (!m_bGUIOpen)
		return;

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

	// 이름 바꾸기
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

	// 삭제
	ImGui::SameLine();
	if (ImGui::Button("Del##DeleteGameObject")) {
		CObject::DestroyGameObjectEvn(m_pTargetObject);
		m_eMode = E_InspectorUIMode::None;
	}

	// 프리펩으로 만들기
	if (ImGui::Button("Make Prefab##Make Prefab")) {
		m_pTargetObject->RegisterAsPrefab(m_pTargetObject->GetName());
	}

	// 복사
	if (ImGui::Button("Clone##Clone GameObject")) {
		CGameObject* pCloneObj = m_pTargetObject->Clone();
		CObject::CreateGameObjectEvn(pCloneObj, m_pTargetObject->GetLayer());

		tstring strName = pCloneObj->GetName();
		if (strName.find(_T("Clone")) == tstring::npos)
			strName = strName + _T("Clone");

		constexpr int iBuffSize = 255;
		TCHAR szBuffer[iBuffSize] = _T("");

		// 고유 이름값 생성
		int id = 1;
		if (m_pTargetObject->GetParentObject()) {
			while (true) {
				_stprintf_s(szBuffer, iBuffSize, _T("%s%d"), strName.c_str(), id++);
				bool found = m_pTargetObject->FindGameObjectSameLine(szBuffer);
				if (!found)
					break;
			}
			CObject::AddChildGameObjectEvn(m_pTargetObject->GetParentObject(), pCloneObj);
		}
		else {
			while (true) {
				_stprintf_s(szBuffer, iBuffSize, _T("%s%d"), strName.c_str(), id++);
				CGameObject* pObj = CSceneManager::GetInstance()->GetCurScene()->FindGameObject(szBuffer);
				if (nullptr == pObj)
					break;
			}
		}
		pCloneObj->SetName(szBuffer);
	}

	// 활성화  세팅
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

	// 레이어 변경
	{
		int iLayer = (int)m_pTargetObject->GetLayer();
		tstring tStrLayerName = LayerToString((E_Layer)iLayer);
		string strLayerName{};
		TStringToString(tStrLayerName, strLayerName);
		ImGui::Text("Cur Layer %d [%s]", iLayer, strLayerName.c_str());
	}
	static int iSetLayer = -1;
	{
		string strLayerName = "NONE";
		if (iSetLayer > 0) {
			tstring tStrLayerName = LayerToString((E_Layer)iSetLayer);
			TStringToString(tStrLayerName, strLayerName);
		}
		
		ImGui::Text("Set Layer [%s]", strLayerName.c_str());
		ImGui::InputInt("##InspectorGUI LayerInput", &iSetLayer);
	}
	
	
	iSetLayer = CMyMath::Clamp(iSetLayer, 0, MAX_SIZE_LAYER - 1);
	if (ImGui::Button("Sel##LayerSelect")) {
		// 레이어를 바꾼다.
		// 자신부터 자식까지 모두 바꾸기
		CObject::ChangeLayerEvn(m_pTargetObject, iSetLayer);
	}
	ImGui::Separator();

	// 태그 변경
	{
		int iTag = (int)m_pTargetObject->GetTag();
		tstring tStrTagName = TagToString((E_Tag)iTag);
		string strTagName{};
		TStringToString(tStrTagName, strTagName);
		ImGui::Text("Cur Tag %d [%s]", iTag, strTagName.c_str());
	}
	static int iSetTag = 0;
	{
		tstring tStrTagName = TagToString((E_Tag)iSetTag);
		string strTagName;
		TStringToString(tStrTagName, strTagName);
		ImGui::Text("Set Tag [%s]", strTagName.c_str());
		ImGui::InputInt("##InspectorGUI Tag Input", &iSetTag);
	}
	
	ImGui::SameLine();
	
	iSetTag = CMyMath::Clamp(iSetTag, 0, max(0, (int)E_Tag::End - 1));

	static bool bWithChild = false;
	if (ImGui::Button("Sel##TagSelect")) {
		m_pTargetObject->SetTag((UINT)iSetTag, bWithChild);
	}
	ImGui::PushID(CImGuiManager::GetInstance()->GetWidgetID());
	ImGui::Checkbox("with child##SelTag", &bWithChild);
	ImGui::PopID();

	if (m_pTargetObject->MeshRenderer()) {
		bool bShadow = m_pTargetObject->IsDynamicShadow();
		ImGui::PushID(CImGuiManager::GetInstance()->GetWidgetID());
		if (ImGui::Checkbox("Dynamic Shadow##DynamicShadowSel", &bShadow))
			m_pTargetObject->SetDynamicShadow(bShadow);
		ImGui::PopID();
	}

	ImGui::Text("FrustumCulling");
	bool bFrustumCulling = m_pTargetObject->IsFrustumCulling();
	ImGui::PushID(CImGuiManager::GetInstance()->GetWidgetID());
	if (ImGui::Checkbox("Frustum Culling##GameObject", &bFrustumCulling))
		m_pTargetObject->SetFrustumCulling(bFrustumCulling);
	ImGui::PopID();

	// FIXED(Jang) : 테스트용
	ImGui::Text("ID : [%s]", to_string(m_pTargetObject->GetUUID()).c_str());
	tstring strLocalAddress = m_pTargetObject->GetLocalAddressTotal().c_str();
	string strLocalAdr{};
	TStringToString(strLocalAddress, strLocalAdr);
	ImGui::Text("Local Address : [%s]", strLocalAdr.c_str());
	
	// ComponentGUI 보여주기
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		if (nullptr == m_arrComGUI[i])
			continue;
		m_arrComGUI[i]->Update();
	}

	// 스크립트 GUI 보여주기
	const vector<CScript*>& vecScript = m_pTargetObject->GetScripts();
	_ResizeScriptGUICnt();
	for (int i = 0; i < (int)vecScript.size(); ++i) {
		ScriptGUI* pScriptGUI = m_vecScriptGUI[i];
		pScriptGUI->SetTargetObject(m_pTargetObject);
		pScriptGUI->SetScript(vecScript[i]);
		pScriptGUI->Update();
	}

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
	if (nullptr == m_pTargetResource)
		return;
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
	
	// 컴포넌트 GUI 설정
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		// FIXED : TODO (Jang) : 모든 컴포넌트를 다 만들면 필요없음.
		if (nullptr == m_arrComGUI[i])
			continue;

		m_arrComGUI[i]->SetTargetObject(_pTargetObj);
	}

	// 스크립트 GUI 설정
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
