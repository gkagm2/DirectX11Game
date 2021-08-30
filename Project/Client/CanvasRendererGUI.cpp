#include "pch.h"
#include "CanvasRendererGUI.h"
#include "GUI.h"
#include <Engine\CCanvasRenderer.h>
#include "ListViewGUI.h"
#include "CImGuiManager.h"
#include <Engine\CResourceManager.h>

CanvasRendererGUI::CanvasRendererGUI() :
	ComponentGUI(E_ComponentType::CanvasRenderer)
{
}

CanvasRendererGUI::~CanvasRendererGUI()
{
}

void CanvasRendererGUI::Update()
{
	if (false == Start())
		return;

	CCanvasRenderer* pCanvasRenderer = GetTargetObject()->CanvasRenderer();
	SharedPtr<CMesh> pMesh = pCanvasRenderer->GetMesh();
	SharedPtr<CMaterial> pMaterial = pCanvasRenderer->GetSharedMaterial();

	char strMeshName[255];
	GetResourceName(pMesh.Get(), strMeshName, 255);

	char strMaterialName[255];
	GetResourceName(pMaterial.Get(), strMaterialName, 255);
	ImGui::Text("Mesh    ");
	ImGui::SameLine();
	ImGui::InputText("##Mesh", strMeshName, 255, ImGuiInputTextFlags_ReadOnly);

	//if (ImGui::Button("Select##Mesh")) {
	//	// ����Ʈ �並 �����ش�.
	//	ListViewGUI* pListViewGUI = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
	//	assert(pListViewGUI);


	//	vector<tstring> vecNames;
	//	CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Mesh, vecNames);
	//	pListViewGUI->SetList(vecNames, ResourceTypeToStr(E_ResourceType::Mesh));
	//	pListViewGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&CanvasRendererGUI::_SetMesh);
	//	pListViewGUI->SetActive(true);
	//}


	ImGui::Text("Material");
	ImGui::SameLine();
	ImGui::InputText("##Material", strMaterialName, 255, ImGuiInputTextFlags_ReadOnly);
	
	//if (ImGui::Button("Select##Material")) {
	//	// ����Ʈ �並 �����ش�.
	//	ListViewGUI* pListViewGUI = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
	//	assert(pListViewGUI);

	//	vector<tstring> vecNames;
	//	CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Material, vecNames);
	//	pListViewGUI->SetList(vecNames, ResourceTypeToStr(E_ResourceType::Material));
	//	pListViewGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&CanvasRendererGUI::_SetMatrial);
	//	pListViewGUI->SetActive(true);
	//}

	//ImGui::SetNextItemWidth(100);
	End();
}