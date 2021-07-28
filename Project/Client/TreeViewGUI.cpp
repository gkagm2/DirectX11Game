#include "pch.h"
#include "TreeViewGUI.h"

// ------------
// TreeViewNode 
// ------------
TreeViewNode::TreeViewNode() :
	m_pOwner(nullptr),
	m_pParentNode(nullptr),
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

	// TODO (Jang) : 선택시 눌려지도록 만들기.
	if (m_pOwner) {
		if (!m_pOwner->IsFrameRender() ||
			m_pOwner->IsRootRender() && !m_pParentNode ||
			m_pOwner->IsRootRender() && m_pParentNode == m_pOwner->GetRootNode()) {
			//m_iStyleFlag |= ImGuiTreeNodeFlags_Framed;
		}
	}

	if (m_bUseFrame)
		m_iStyleFlag |= ImGuiTreeNodeFlags_Framed;
	if (!hasChild)
		m_iStyleFlag |= ImGuiTreeNodeFlags_Leaf;
	if (this == m_pOwner->m_pSelectedNode)
		m_iStyleFlag |= ImGuiTreeNodeFlags_Selected;

	// Node Update
	string strName = m_strName;
	if (strName.empty())
		strName = "Game Object";
	
	/*char szBuffer[255] = {};
	sprintf(szBuffer, "##%ld", m_dwData);*/
	strName = strName + "##" + std::to_string(m_dwData);

	if (ImGui::TreeNodeEx(strName.c_str(), m_iStyleFlag)) {
		// 해당 아이템이 클릭된 경우
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
			if (this != m_pOwner->m_pSelectedNode) {
				if (nullptr != m_pOwner->m_pSelectedNode) {

				}
				m_pOwner->_SetSelectedNode(this);
			}
		}

		for (UINT i = 0; i < m_vecChildNodes.size(); ++i)
			m_vecChildNodes[i]->Update();
		ImGui::TreePop();
	}
}


// ------------
// TreeViewGUI 
// ------------
TreeViewGUI::TreeViewGUI() :
	m_pRootNode(nullptr),
	m_pSelectedNode(nullptr),
	m_bRootRender(false),
	m_bFrameUse(false),
	m_bFrameOnlyParent(false),
	m_pSelectFunc{ nullptr },
	m_PDragDropFunc{ nullptr }
{
}

TreeViewGUI::~TreeViewGUI()
{
	if (m_pRootNode)
		delete m_pRootNode;
}

void TreeViewGUI::Init()
{
}

void TreeViewGUI::Update()
{
	if (nullptr == m_pRootNode)
		return;

	if (m_bRootRender)
		m_pRootNode->Update();
	else {
		const vector<TreeViewNode*>& vecChildNodes = m_pRootNode->GetChildNodes();
		for (UINT i = 0; i < vecChildNodes.size(); ++i)
			vecChildNodes[i]->Update();
	}
}

TreeViewNode* TreeViewGUI::AddItem(const string& _str, DWORD_PTR _dwData, TreeViewNode* _pParent)
{

	TreeViewNode* pNewNode = new TreeViewNode;
	pNewNode->SetName(_str);
	pNewNode->SetData(_dwData);
	pNewNode->_SetOwner(this);

	if (nullptr == _pParent) {
		assert(!m_pRootNode); // 존재하면 안됨.
		m_pRootNode = pNewNode;
	}
	else
		_pParent->AddChild(pNewNode);

	return pNewNode;
}

void TreeViewGUI::Clear()
{
	if (m_pRootNode)
		delete m_pRootNode;
	m_pRootNode = nullptr;
}