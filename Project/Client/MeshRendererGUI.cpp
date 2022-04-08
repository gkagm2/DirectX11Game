#include "pch.h"
#include "CImGuiManager.h"
#include "MeshRendererGUI.h"
#include "ListViewGUI.h"
#include <Engine\CMeshRenderer.h>
#include <Engine\CResourceManager.h>

MeshRendererGUI::MeshRendererGUI() :
	ComponentGUI(E_ComponentType::MeshRenderer)
{
}

MeshRendererGUI::~MeshRendererGUI()
{
}

void MeshRendererGUI::Update()
{
	if (false == Start())
		return;

	CMeshRenderer* pMeshRenderer = GetTargetObject()->MeshRenderer();
	SharedPtr<CMesh> pMesh = pMeshRenderer->GetMesh();
	SharedPtr<CMaterial> pMaterial = pMeshRenderer->GetSharedMaterial(0);

	char strMeshName[255];
	GetResourceName(pMesh.Get(), strMeshName, 255);

	char strMaterialName[255];
	GetResourceName(pMaterial.Get(), strMaterialName, 255);
		ImGui::Text("Mesh    "); 
	ImGui::SameLine(); 
	ImGui::InputText("##Mesh", strMeshName, 255, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("Select##Mesh")) {
		// 리스트 뷰를 보여준다.
		ListViewGUI* pListViewGUI = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
		assert(pListViewGUI);

		
		vector<tstring> vecNames;
		CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Mesh, vecNames);
		pListViewGUI->SetList(vecNames, ResourceTypeToStr(E_ResourceType::Mesh));
		pListViewGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&MeshRendererGUI::_SetMesh);
		pListViewGUI->SetActive(true);
	}


	ImGui::Text("Material"); 
	ImGui::SameLine(); 
	ImGui::InputText("##Material", strMaterialName, 255, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("Select##Material")) {
		// 리스트 뷰를 보여준다.
		ListViewGUI* pListViewGUI = dynamic_cast<ListViewGUI*>(CImGuiManager::GetInstance()->FindGUI(STR_GUI_ListView));
		assert(pListViewGUI);

		vector<tstring> vecNames;
		CResourceManager::GetInstance()->GetResourceKeys(E_ResourceType::Material, vecNames);
		pListViewGUI->SetList(vecNames, ResourceTypeToStr(E_ResourceType::Material));
		pListViewGUI->SetDoubleClickCallBack(this, (GUI_CALLBACK)&MeshRendererGUI::_SetMatrial);
		pListViewGUI->SetActive(true);
	}

	//ImGui::SetNextItemWidth(100);
	End();
}

void MeshRendererGUI::_SetMesh(DWORD_PTR _dw1, DWORD_PTR _dw)
{
	string strKey = (const char*)_dw1;
	tstring tstrKey; 
	StringToTString(strKey, tstrKey);
	SharedPtr<CMesh> pMesh = CResourceManager::GetInstance()->FindRes<CMesh>(tstrKey);
	assert(pMesh.Get());
	GetTargetObject()->MeshRenderer()->SetMesh(pMesh);
}

void MeshRendererGUI::_SetMatrial(DWORD_PTR _dw1, DWORD_PTR _dw)
{
	string strKey = (const char*)_dw1;
	tstring tstrKey;
	StringToTString(strKey, tstrKey);
	SharedPtr<CMaterial> pMtrl = CResourceManager::GetInstance()->FindRes<CMaterial>(tstrKey);
	assert(pMtrl.Get());
	GetTargetObject()->MeshRenderer()->SetMaterial(pMtrl, 0);
}