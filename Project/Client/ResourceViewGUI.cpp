#include "pch.h"
#include "ResourceViewGUI.h"
#include <Engine\CResourceManager.h>
#include "InspectorGUI.h"
#include "CImGuiManager.h"

ResourceViewGUI::ResourceViewGUI()
{
	SetName("Resource");
}

ResourceViewGUI::~ResourceViewGUI()
{
}

void ResourceViewGUI::Init()
{
	m_treeView.SetFrameRender(true);
	m_treeView.SetFrameOnlyParent(true);
	m_treeView.SetRootRender(false);

	m_treeView.SetSelectCallBack(this, (SEL_CHANGE_CALLBACK)&ResourceViewGUI::SelectResource);

	_RenewTreeView();
}

void ResourceViewGUI::Update()
{
	_RenewResource();

	ImGui::Begin(GetName().c_str(), &m_bGUIOpen);
	m_treeView.Update();
	ImGui::End();
}

void ResourceViewGUI::_RenewResource()
{
	// 리소스 변경이 탐지되면 
	if (CResourceManager::GetInstance()->IsFixed())
		_RenewTreeView(); // 리소스 목록 추가	
}

void ResourceViewGUI::_RenewTreeView()
{
	m_treeView.Clear();

	TreeViewNode* pRoot = m_treeView.AddItem("RootNode", 0, nullptr);

	tstring tstr;
	string str;
	bool bUseFrame = true;

	for (UINT i = 0; i < (UINT)E_ResourceType::End; ++i) {
		const unordered_map<tstring, CResource*>& umapResources = CResourceManager::GetInstance()->GetResources((E_ResourceType)i);
		TStringToString(ResourceTypeToStr((E_ResourceType)i), str);
		TreeViewNode* pNode = m_treeView.AddItem(str, 0, pRoot);

		for (const auto& pair : umapResources) {
			string strKey;
			TStringToString(pair.first, strKey);
			m_treeView.AddItem(strKey, (DWORD_PTR)pair.second, pNode);
		}
	}
}

void ResourceViewGUI::SelectResource(TreeViewNode* _pNode)
{
	if (0 == _pNode->GetData())
		return;

	// InspectorGUI에 선택된 리소스를 알린다.
	InspectorGUI* pInspector = (InspectorGUI*)CImGuiManager::GetInstance()->FindGUI(STR_GUI_Inspector);
	pInspector->SetTargetResource((CResource*)_pNode->GetData());
}