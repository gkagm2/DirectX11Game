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

	//CCanvasRenderer* pCanvasRenderer = GetTargetObject()->CanvasRenderer();

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