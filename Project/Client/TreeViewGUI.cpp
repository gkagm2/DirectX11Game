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
	m_vTextColor{ 1.f,1.f,1.f,1.f },
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
	if (m_vecChildNodes.empty() && !m_bUseFrame) // 자식이 없는 경우 화살표 표시 X
		m_iStyleFlag |= ImGuiTreeNodeFlags_Leaf;
	if (this == m_pOwner->_GetSelectdItem())
		m_iStyleFlag |= ImGuiTreeNodeFlags_Selected;
	if (this == m_pOwner->_GetDragStartItem())
		m_iStyleFlag |= ImGuiTreeNodeFlags_Selected;

	bool bLeftClicked = false;

	if (dynamic_cast<CGameObject*>((CGameObject*)m_dwData)) {
		if (((CGameObject*)m_dwData)->IsActive())
			SetTextColor(ImVec4(1.f, 1.f, 1.f, 1.f));
		else
			SetTextColor(ImVec4(0.4f, 0.4f, 0.4f, 1.f));
	}

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

	// Node가 열려있을 경우
	if (ImGui::TreeNodeEx(strName.c_str(), m_iStyleFlag)) {
		bLeftClicked = _IsLeftClick();
		_DragDrop();
		_DoubleClick();

		// 자식 노드 업데이트
		for (size_t i = 0; i < m_vecChildNodes.size(); ++i)
			m_vecChildNodes[i]->Update();

		ImGui::TreePop();
	}
	else { // Node가 닫혀있을 경우
		// 왼쪽 클릭 체크
		bLeftClicked = _IsLeftClick();
		// 드래그 드랍 체크
		_DragDrop();
		_DoubleClick();
	}

	// 이벤트 처리
	if (bLeftClicked)
		m_pOwner->_ExcuteClickedCallBack(this);

	// 마우스 뗐을 때 눌린 상태 되돌려 놓기
	if (InputKeyRelease(E_Key::LBUTTON))
		m_bMouseLBtnPress = false;

	ImGui::PopStyleColor();
}


void TreeViewNode::_DragDrop()
{
	//// 해당 아이템이 드래그 시작한 경우
	// 드래그 시작
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
		string name = m_pOwner->GetName();
		ImGui::SetDragDropPayload(m_pOwner->GetName().c_str(), &m_dwData, sizeof(DWORD_PTR));
		ImGui::Text(GetName().c_str());
		ImGui::EndDragDropSource();
		m_pOwner->_SetDragStartNode(this);
	}


	// 마우스가 drop 타겟으로 이동했으면 실행
	if (ImGui::BeginDragDropTarget()) {
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_pOwner->GetName().c_str());
		// 해당 아이템이 드롭한 경우
		if (payload) {
			m_pOwner->_SetDropTargetNode(this);
			// Drag Drop 발생
			if (m_pOwner->m_pDragStartNode && m_pOwner->m_pDropTargetNode)
			{
				((m_pOwner->m_pDragDropInst)->*m_pOwner->m_pDragDropFunc)(m_pOwner->m_pDragStartNode, m_pOwner->m_pDropTargetNode);

				m_pOwner->_SetDragStartNode(nullptr);
				m_pOwner->_SetDropTargetNode(nullptr);
			}
		}
		ImGui::EndDragDropTarget();
	}

	// 아이템을 드롭했는데 비어있는 창이면
	if (InputKeyRelease(E_Key::LBUTTON) &&
		nullptr != m_pOwner->m_pDragStartNode &&
		nullptr == m_pOwner->m_pDropTargetNode &&
		nullptr != m_pOwner->m_pDragDropInst &&
		nullptr != m_pOwner->m_pDragDropFunc &&
		true == GUI::IsMouseInWindowContentRegion()) {
		// drag 시작한 트리노드의 정보를 가져옴
		m_pOwner->_SetDropTargetNode(this);
		((m_pOwner->m_pDragDropInst)->*m_pOwner->m_pDragDropFunc)(m_pOwner->m_pDragStartNode, nullptr);

		// FIXED(Jang) : BeginDragDropTarget 타이밍이 안맞음... 일단 주석처리
		m_pOwner->_SetDragStartNode(nullptr);
		m_pOwner->_SetDropTargetNode(nullptr);
	}
}

void TreeViewNode::_DoubleClick()
{
	// 아이템을 더블클릭 했을 경우
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
	// 아이템 왼쪽 클릭 된 경우
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
#include "GUI.h"
TreeViewGUI::TreeViewGUI(const string& _strName) :
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
	SetName(_strName);
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
		assert(!m_pRootNode); // 존재하면 안됨.
		m_pRootNode = pNewNode;
	}
	else
		_pParent->AddChild(pNewNode);

	return pNewNode;
}

void TreeViewGUI::Clear()
{
	SAFE_DELETE(m_pRootNode);

	m_pSelectedNode = nullptr;

	m_pClickedFunc = nullptr;
	m_pClickedInst = nullptr;

	m_pDragDropFunc = nullptr;
	m_pDragDropInst = nullptr;

	m_pDBCallBack = nullptr;
	m_pDBCInst = nullptr;
	m_pGDBCCallBack = nullptr;
}