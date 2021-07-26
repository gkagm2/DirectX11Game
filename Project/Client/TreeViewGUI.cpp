#include "pch.h"
#include "TreeViewGUI.h"

// ------------
// TreeViewNode 
// ------------
TreeViewNode::TreeViewNode() :
	m_dwData(0),
	m_bUseFrame(false),
	m_iStyleFlag(0)
{
}

TreeViewNode::~TreeViewNode()
{
	Safe_Delete_Vector(m_vecChildNodes);
}

void TreeViewNode::Update()
{
	// Node의 옵션 설정
	bool hasChild = true;
	if (m_vecChildNodes.empty())
		hasChild = false;

	m_iStyleFlag = ImGuiTreeNodeFlags_None;
	if (m_bUseFrame)
		m_iStyleFlag |= ImGuiTreeNodeFlags_Framed;
	if (!hasChild)
		m_iStyleFlag |= ImGuiTreeNodeFlags_Leaf;

	// Node Update
	if (ImGui::TreeNodeEx(m_strName.c_str(), m_iStyleFlag)) {
		for (UINT i = 0; i < m_vecChildNodes.size(); ++i)
			m_vecChildNodes[i]->Update();
		ImGui::TreePop();
	}
}



// ------------
// TreeViewGUI 
// ------------
TreeViewGUI::TreeViewGUI() :
	m_pRoot(nullptr),
	m_bRootRender(false)
{
}

TreeViewGUI::~TreeViewGUI()
{
	if (m_pRoot)
		delete m_pRoot;
}

void TreeViewGUI::Init()
{
}

void TreeViewGUI::Update()
{
	if (nullptr == m_pRoot)
		return;

	if (m_bRootRender)
		m_pRoot->Update();
	else {
		const vector<TreeViewNode*>& vecChildNodes = m_pRoot->GetChildNodes();
		for (UINT i = 0; i < vecChildNodes.size(); ++i)
			vecChildNodes[i]->Update();
	}
}

TreeViewNode* TreeViewGUI::AddItem(const string& _str, DWORD_PTR _dwData, TreeViewNode* _pParent)
{
	TreeViewNode* pNewNode = new TreeViewNode;
	pNewNode->SetName(_str);
	pNewNode->SetData(_dwData);

	if (nullptr == _pParent) {
		assert(!m_pRoot); // 존재하면 안됨.
		m_pRoot = pNewNode;
	}
	else
		_pParent->AddChild(pNewNode);

	return pNewNode;
}
 
TreeViewNode* TreeViewGUI::AddItem(const string& _str, DWORD_PTR _dwData, bool _bUseFrame, TreeViewNode* _pParent)
{
	TreeViewNode* pNewNode = AddItem(_str, _dwData, _pParent);
	pNewNode->UseFrame(_bUseFrame);
	return pNewNode;
}

void TreeViewGUI::Clear()
{
	if (m_pRoot)
		delete m_pRoot;
	m_pRoot = nullptr;
}