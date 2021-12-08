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
	m_iStyleFlag(0),
	m_vTextColor{1.f,1.f,1.f,1.f}
{
}

TreeViewNode::~TreeViewNode()
{
	Safe_Delete_Vector(m_vecChildNodes);
}

void TreeViewNode::Update()
{


	m_iStyleFlag = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;

	/*if (m_pOwner) {
		if (!m_pOwner->IsFrameRender() ||
			m_pOwner->IsRootRender() && !m_pParentNode ||
			m_pOwner->IsRootRender() && m_pParentNode == m_pOwner->GetRootNode()) {
			m_iStyleFlag |= ImGuiTreeNodeFlags_Framed;
		}
	}*/

	if (m_bUseFrame)
		m_iStyleFlag |= ImGuiTreeNodeFlags_Framed;
	if (m_vecChildNodes.empty()) // �ڽ��� ���� ��� ȭ��ǥ ǥ�� X
		m_iStyleFlag |= ImGuiTreeNodeFlags_Leaf;
	if (this == m_pOwner->m_pSelectedNode)
		m_iStyleFlag |= ImGuiTreeNodeFlags_Selected;


	// Node Update
	string strName = m_strName;
	if (strName.empty())
		strName = "[No Name]";
	
	/*char szBuffer[255] = {};
	sprintf(szBuffer, "##%ld", m_dwData);*/
	strName = strName + "##" + std::to_string(m_dwData);

	ImGui::PushStyleColor(ImGuiCol_Text, m_vTextColor);
	
	if (ImGui::TreeNodeEx(strName.c_str(), m_iStyleFlag)) {
		// �ش� �������� Ŭ���� ���
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
			// ���� �������� ���õ��� �ʾ�����
			//if (this != m_pOwner->m_pSelectedNode) {
				m_pOwner->_SetSelectedNode(this); // ����

				// ���õ� �ν��Ͻ��� �����ϸ�
				if (nullptr != m_pOwner->m_pSelectedNode && m_pOwner->m_pSelectInst) {
					((m_pOwner->m_pSelectInst)->*m_pOwner->m_pSelectFunc)(m_pOwner->m_pSelectedNode);
				}
			//}
		}

		// �ش� ������ ������ ��ư�� Ŭ���� ���
		//if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(2)) {
		//	// ���� �������� ���õ��� �ʾ�����
		//	if (this != m_pOwner->m_pSelectedNode) {
		//		m_pOwner->_SetSelectedNode(this);

		//		// ���õ� �ν��Ͻ��� �����ϸ�
		//		if (nullptr != m_pOwner->m_pSelectedNode && m_pOwner->m_pSelectInst) {
		//			((m_pOwner->m_pSelectInst)->*m_pOwner->m_pSelectFunc)(m_pOwner->m_pSelectedNode);

		//			// ContextMenu Open
		//			
		//		}
		//	}
		//}
		
		// �������� ����Ŭ�� ���� ���
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
		{
			if (m_pOwner->m_pDBCInst && m_pOwner->m_pDBCallBack) {
				string name = GetName();
				std::shared_ptr<string> pName = std::make_shared<string>(name);
				(m_pOwner->m_pDBCInst->*m_pOwner->m_pDBCallBack)((DWORD_PTR)pName.get(), 0);
			}
		}

		// �ش� �������� �巡�� ������ ���
		if (ImGui::BeginDragDropSource()) {
			// �ڽ��� �巡�� ���� ��带 �˸�
			m_pOwner->_SetDragStartNode(this);
			ImGui::SetDragDropPayload("DraggedNode", this, sizeof(TreeViewNode));
			ImGui::Text("This is a drag and drop source");
			ImGui::EndDragDropSource();
		}
		
		

		// �ش� �������� ����� ���
		if (ImGui::BeginDragDropTarget()) {
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DraggedNode");

			if (payload) {
				// �ڽ��� �巡�� �������� �˸�
				m_pOwner->_SetDropTargetNode(this);

				// drag ������ Ʈ������� ������ ������
				TreeViewNode* pDragNode = (TreeViewNode*)payload->Data;
				// drag drop callback ȣ��
				((m_pOwner->m_pDragDropInst)->*m_pOwner->m_pDragDropFunc)(pDragNode, this);

				// tree�� drag����, Ÿ�� ���� �ʱ�ȭ
				m_pOwner->_SetDragStartNode(nullptr);
				m_pOwner->_SetDropTargetNode(nullptr);
			}

			ImGui::EndDragDropTarget();
		}
		// �������� ����ߴµ� ����ִ� â�̸�
		else if (ImGui::IsMouseReleased(0) && m_pOwner->m_pDraggedNode && !m_pOwner->m_pDropTargetNode) {
			if (m_pOwner->m_pDragDropInst && m_pOwner->m_pDragDropFunc) {
				if (GUI::IsMouseInWindowContentRegion()) {
					// drag ������ Ʈ������� ������ ������
					m_pOwner->_SetDropTargetNode(this);
					((m_pOwner->m_pDragDropInst)->*m_pOwner->m_pDragDropFunc)(m_pOwner->m_pDraggedNode, nullptr);
					m_pOwner->_SetDragStartNode(nullptr);
					m_pOwner->_SetDropTargetNode(nullptr);
				}
			}
		}

		for (UINT i = 0; i < m_vecChildNodes.size(); ++i)
			m_vecChildNodes[i]->Update();
		ImGui::TreePop();
	}
	ImGui::PopStyleColor();
}


// ------------
// TreeViewGUI 
// ------------
TreeViewGUI::TreeViewGUI() :
	m_pRootNode(nullptr),
	m_pSelectedNode(nullptr),
	m_pDraggedNode(nullptr),
	m_pDropTargetNode(nullptr),
	m_bRootRender(false),
	m_bFrameUse(false),
	m_bFrameOnlyParent(false),
	m_pSelectFunc{ nullptr },
	m_pSelectInst(nullptr),
	m_pDragDropFunc{ nullptr },
	m_pDragDropInst(nullptr),
	m_pDBCallBack{ nullptr },
	m_pDBCInst{ nullptr },
	m_pGDBCCallBack{ nullptr }
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
}