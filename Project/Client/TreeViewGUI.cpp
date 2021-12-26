#include "pch.h"
#include "TreeViewGUI.h"
#include <Engine\CKeyManager.h>
#include "DebugGUI.h"
#include "CImGuiManager.h"
#include <Engine\CGameObject.h>

// ------------
// TreeViewNode 
// ------------
TreeViewNode::TreeViewNode() :
	m_pOwner(nullptr),
	m_pParentNode(nullptr),
	m_dwData(0),
	m_bUseFrame(false),
	m_iStyleFlag(0),
	m_vTextColor{1.f,1.f,1.f,1.f},
	m_bOpen{ false },
	m_bMouseLBtnPress(false)
{
}

TreeViewNode::~TreeViewNode()
{
	Safe_Delete_Vector(m_vecChildNodes);
}

void TreeViewNode::Update()
{
	int m_iStyleFlag = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

	if (m_bUseFrame)
		m_iStyleFlag |= ImGuiTreeNodeFlags_Framed;
	if (m_vecChildNodes.empty() && !m_bUseFrame) // �ڽ��� ���� ��� ȭ��ǥ ǥ�� X
		m_iStyleFlag |= ImGuiTreeNodeFlags_Leaf;
	if (this == m_pOwner->_GetSelectdItem())
		m_iStyleFlag |= ImGuiTreeNodeFlags_Selected;
	if (this == m_pOwner->_GetDragStartItem())
		m_iStyleFlag |= ImGuiTreeNodeFlags_Selected;


	bool bLeftClicked = false;

	// Node Update
	string strName = m_strName;
	if (strName.empty())
		strName = "[No Name]";
	
	/*char szBuffer[255] = {};
	sprintf(szBuffer, "##%ld", m_dwData);*/
	strName = strName + "##" + std::to_string(m_dwData);

	ImGui::PushStyleColor(ImGuiCol_Text, m_vTextColor);

	
	if (m_bOpen) {
		ImGui::SetNextItemOpen(m_bOpen);
		m_bOpen = false;
	}

	// Node�� �������� ���
	if (ImGui::TreeNodeEx(strName.c_str(), m_iStyleFlag)) {
		bLeftClicked = _IsLeftClick();
		_DragDrop();
		_DoubleClick();

		// �ڽ� ��� ������Ʈ
		for (size_t i = 0; i < m_vecChildNodes.size(); ++i)
			m_vecChildNodes[i]->Update();

		ImGui::TreePop();
	}
	else { // Node�� �������� ���
		// ���� Ŭ�� üũ
		bLeftClicked = _IsLeftClick();
		// �巡�� ��� üũ
		_DragDrop();
		_DoubleClick();
	}

	// �̺�Ʈ ó��
	if (bLeftClicked)
		m_pOwner->_ExcuteClickedCallBack(this);

	// ���콺 ���� �� ���� ���� �ǵ��� ����
	if (InputKeyRelease(E_Key::LBUTTON))
		m_bMouseLBtnPress = false;

	ImGui::PopStyleColor();
}


void TreeViewNode::_DragDrop()
{
	//// �ش� �������� �巡�� ������ ���
	// �巡�� ����
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
		ImGui::SetDragDropPayload(m_pOwner->GetName().c_str(), &m_dwData, sizeof(DWORD_PTR));
		ImGui::Text(GetName().c_str());
		ImGui::EndDragDropSource();
		m_pOwner->_SetDragStartNode(this);
	}


	// ���콺�� drop Ÿ������ �̵������� ����
	if (ImGui::BeginDragDropTarget()) {
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_pOwner->GetName().c_str());
		// �ش� �������� ����� ���
		if (payload) {
			m_pOwner->_SetDropTargetNode(this);
			// Drag Drop �߻�
			if (m_pOwner->m_pDragStartNode && m_pOwner->m_pDropTargetNode)
			{
				((m_pOwner->m_pDragDropInst)->*m_pOwner->m_pDragDropFunc)(m_pOwner->m_pDragStartNode, m_pOwner->m_pDropTargetNode);

				m_pOwner->_SetDragStartNode(nullptr);
				m_pOwner->_SetDropTargetNode(nullptr);
			}
		}
		ImGui::EndDragDropTarget();
	}

	// �������� ����ߴµ� ����ִ� â�̸�
	if (InputKeyRelease(E_Key::LBUTTON) &&
		nullptr != m_pOwner->m_pDragStartNode &&
		nullptr == m_pOwner->m_pDropTargetNode &&
		nullptr != m_pOwner->m_pDragDropInst &&
		nullptr != m_pOwner->m_pDragDropFunc &&
		true == GUI::IsMouseInWindowContentRegion()) {
		// drag ������ Ʈ������� ������ ������
		m_pOwner->_SetDropTargetNode(this);
		((m_pOwner->m_pDragDropInst)->*m_pOwner->m_pDragDropFunc)(m_pOwner->m_pDragStartNode, nullptr);

		// FIXED(Jang) : BeginDragDropTarget Ÿ�̹��� �ȸ���... �ϴ� �ּ�ó��
		m_pOwner->_SetDragStartNode(nullptr);
		m_pOwner->_SetDropTargetNode(nullptr);
	}
}

void TreeViewNode::_DoubleClick()
{
	// �������� ����Ŭ�� ���� ���
	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
		if (m_pOwner->m_pDBCInst && m_pOwner->m_pDBCallBack) {
			string name = GetName();
			std::shared_ptr<string> pName = std::make_shared<string>(name);
			(m_pOwner->m_pDBCInst->*m_pOwner->m_pDBCallBack)((DWORD_PTR)pName.get(), 0);
		}
	}
}

bool TreeViewNode::_IsLeftClick()
{
	// ������ ���� Ŭ�� �� ���
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_None) && InputKeyPress(E_Key::LBUTTON)) {
		m_bMouseLBtnPress = true;
		return false;
	}
	else if (m_bMouseLBtnPress && ImGui::IsItemHovered(ImGuiHoveredFlags_None) && InputKeyRelease(E_Key::LBUTTON))
		return true;
	return false;
}

void TreeViewGUI::_ExcuteClickedCallBack(TreeViewNode* _pClickedItem)
{
	m_pSelectedNode = _pClickedItem;
	if(m_pClickedInst && m_pClickedInst)
		(m_pClickedInst->*m_pClickedFunc)((DWORD_PTR)_pClickedItem, _pClickedItem->m_dwData);
}

// ------------
// TreeViewGUI 
// ------------
TreeViewGUI::TreeViewGUI(const string& _strName) :
	m_strName(_strName),
	m_pRootNode(nullptr),
	m_pSelectedNode(nullptr),
	m_pDragStartNode(nullptr),
	m_pDropTargetNode(nullptr),
	m_bRootRender(false),
	m_bFrameUse(false),
	m_bFrameOnlyParent(false),
	m_pClickedFunc{ nullptr },
	m_pClickedInst{ nullptr },
	m_pDragDropFunc{ nullptr },
	m_pDragDropInst(nullptr),
	m_pDBCallBack{ nullptr },
	m_pDBCInst{ nullptr },
	m_pGDBCCallBack{ nullptr }
{
}

TreeViewGUI::~TreeViewGUI()
{
	Clear();
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

	if (InputKeyRelease(E_Key::LBUTTON))
		m_pDragStartNode = nullptr;
}

TreeViewNode* TreeViewGUI::AddItem(const string& _str, DWORD_PTR _dwData, TreeViewNode* _pParent)
{
	TreeViewNode* pNewNode = new TreeViewNode;
	pNewNode->SetName(_str);
	pNewNode->SetData(_dwData);
	pNewNode->_SetOwner(this);

	if (nullptr == _pParent) {
		assert(!m_pRootNode); // �����ϸ� �ȵ�.
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

	m_pSelectedNode = nullptr;

	m_pClickedFunc = nullptr;
	m_pClickedInst = nullptr;

	m_pDragDropFunc = nullptr;
	m_pDragDropInst = nullptr;

	m_pDBCallBack = nullptr;
	m_pDBCInst = nullptr;
	m_pGDBCCallBack = nullptr;
}