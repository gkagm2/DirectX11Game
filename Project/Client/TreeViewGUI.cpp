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
	if (m_vecChildNodes.empty()) // 자식이 없는 경우 화살표 표시 X
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

	if (ImGui::TreeNodeEx(strName.c_str(), m_iStyleFlag)) {
		// 해당 아이템이 클릭된 경우
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
			// 현재 아이템이 선택되지 않았으면
			if (this != m_pOwner->m_pSelectedNode) {
				m_pOwner->_SetSelectedNode(this); // 선택

				// 선택된 인스턴스가 존재하면
				if (nullptr != m_pOwner->m_pSelectedNode && m_pOwner->m_pSelectInst) {
					((m_pOwner->m_pSelectInst)->*m_pOwner->m_pSelectFunc)(m_pOwner->m_pSelectedNode);
				}
			}
		}

		// 해당 아이템 오른쪽 버튼을 클릭한 경우
		//if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(2)) {
		//	// 현재 아이템이 선택되지 않았으면
		//	if (this != m_pOwner->m_pSelectedNode) {
		//		m_pOwner->_SetSelectedNode(this);

		//		// 선택된 인스턴스가 존재하면
		//		if (nullptr != m_pOwner->m_pSelectedNode && m_pOwner->m_pSelectInst) {
		//			((m_pOwner->m_pSelectInst)->*m_pOwner->m_pSelectFunc)(m_pOwner->m_pSelectedNode);

		//			// ContextMenu Open
		//			
		//		}
		//	}
		//}
		
		// 아이템을 더블클릭 했을 경우
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
		{
			if (m_pOwner->m_pDBCInst && m_pOwner->m_pDBCallBack) {
				string name = GetName();
				std::shared_ptr<string> pName = std::make_shared<string>(name);
				(m_pOwner->m_pDBCInst->*m_pOwner->m_pDBCallBack)((DWORD_PTR)pName.get(), 0);
			}
		}

		// 해당 아이템이 드래그 시작한 경우
		if (ImGui::BeginDragDropSource()) {
			// 자신이 드래그 시작 노드를 알림
			m_pOwner->_SetDragStartNode(this);
			ImGui::SetDragDropPayload("DraggedNode", this, sizeof(TreeViewNode));
			ImGui::Text("This is a drag and drop source");
			ImGui::EndDragDropSource();
		}
		
		

		// 해당 아이템이 드롭한 경우
		if (ImGui::BeginDragDropTarget()) {
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DraggedNode");

			if (payload) {
				// 자신이 드래그 목적지를 알림
				m_pOwner->_SetDropTargetNode(this);

				// drag 시작한 트리노드의 정보를 가져옴
				TreeViewNode* pDragNode = (TreeViewNode*)payload->Data;
				// drag drop callback 호출
				((m_pOwner->m_pDragDropInst)->*m_pOwner->m_pDragDropFunc)(pDragNode, this);

				// tree의 drag시작, 타겟 정보 초기화
				m_pOwner->_SetDragStartNode(nullptr);
				m_pOwner->_SetDropTargetNode(nullptr);
			}

			ImGui::EndDragDropTarget();
		}
		// 아이템을 드롭했는데 비어있는 창이면
		else if (ImGui::IsMouseReleased(0) && m_pOwner->m_pDraggedNode && !m_pOwner->m_pDropTargetNode) {
			if (m_pOwner->m_pDragDropInst && m_pOwner->m_pDragDropFunc) {
				if (GUI::IsMouseInWindowContentRegion()) {
					// drag 시작한 트리노드의 정보를 가져옴
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
	m_pDragDropInst(nullptr)
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