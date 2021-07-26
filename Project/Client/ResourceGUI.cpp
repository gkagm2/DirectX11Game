#include "pch.h"
#include "ResourceGUI.h"
#include <Engine\CResourceManager.h>

ResourceGUI::ResourceGUI() :
	m_treeView{}

{
	SetName("Resource");
}

ResourceGUI::~ResourceGUI()
{
}

void ResourceGUI::Init()
{
	m_treeView.SetFrameRender(true);
	m_treeView.SetFrameOnlyParent(true);
	m_treeView.SetRootRender(false);
	_RenewTreeView();
}

void ResourceGUI::Update()
{
	_RenewResource();

	ImGui::Begin(GetName().c_str(), &m_bGUIOpen);
	m_treeView.Update();
	ImGui::End();
}

void ResourceGUI::_RenewResource()
{
	// 리소스 변경이 탐지되면 
	if (CResourceManager::GetInstance()->IsFixed())
		_RenewResource(); // 리소스 목록 추가	
}

void ResourceGUI::_RenewTreeView()
{
	m_treeView.Clear();

	TreeViewNode* pRoot = m_treeView.AddItem("RootNode", 0, nullptr);
	
	tstring tstr;
	string str;
	bool bUseFrame = true;
	
	for (UINT i = 0; i < (UINT)E_ResourceType::End; ++i) {
		const unordered_map<tstring, CResource*>& umapResources =  CResourceManager::GetInstance()->GetResources((E_ResourceType)i);
		TStringToString(ResourceTypeToStr((E_ResourceType)i), str);
		TreeViewNode* pNode = m_treeView.AddItem(str, 0, pRoot);

		for (const auto& pair : umapResources) {
			string strKey;
			TStringToString(pair.first, strKey);
			m_treeView.AddItem(strKey, (DWORD_PTR)pair.second, pNode);
		}
	}
}