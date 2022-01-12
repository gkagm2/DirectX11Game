#include "pch.h"
#include "HierarchyGUI.h"
#include "TreeViewGUI.h"

#include <Engine\CEventManager.h>
#include <Engine\CSceneManager.h>
#include <Engine\CScene.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>

#include "CImGuiManager.h"
#include "InspectorGUI.h"
#include "ResourceViewGUI.h"
#include "TreeViewGUI.h"

HierarchyGUI::HierarchyGUI() :
	m_treeView(STR_GUI_HierarchyTree)
{
	SetName(STR_GUI_Hierarchy);
}

HierarchyGUI::~HierarchyGUI()
{
}

void HierarchyGUI::Init()
{
	_RenewTreeView();
	m_treeView.SetFrameRender(false);
	m_treeView.SetFrameOnlyParent(false);
}

void HierarchyGUI::Update()
{
	if (!m_bGUIOpen)
		return;

	if (CEventManager::GetInstance()->DidEventHappended())
		_RenewTreeView();
		
	ImGui::Begin(GetName().c_str(), &m_bGUIOpen);
	m_treeView.Update();
	ImGui::End();
}

void HierarchyGUI::_RenewTreeView()
{
	m_treeView.Clear();
	TreeViewNode* pTreeViewRoot = m_treeView.AddItem("RootHierachy", 0, nullptr);

	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
	
	//  씬에서 최상위 오브젝트들을 가져옴
	vector<CGameObject*> vecRootObjs;
	pCurScene->GetRootGameObjects(vecRootObjs);

	// DFS를 이용한 순환
	list<std::pair<CGameObject*, TreeViewNode*>> stk; // stk : stack
	for (UINT j = 0; j < vecRootObjs.size(); ++j) {
		UINT vecSize = (UINT)vecRootObjs.size();

		stk.push_back(std::make_pair(vecRootObjs[j], pTreeViewRoot));

		while (!stk.empty()) {
			CGameObject* pObj = stk.back().first;
			TreeViewNode* pNode = stk.back().second;
			stk.pop_back();

			string strObjName;
			TStringToString(pObj->GetName(), strObjName);
			pNode = m_treeView.AddItem(strObjName, (DWORD_PTR)pObj, pNode);

			const vector<CGameObject*>& vecChilds = pObj->GetChildsObject();
			for (UINT k = 0; k < vecChilds.size(); ++k)
				stk.push_back(std::make_pair(vecChilds[k], pNode));
		}
	}

	m_treeView.SetClickCallBack(this, TREE_CALLBACK(&HierarchyGUI::_ClickedGameObject));
	m_treeView.SetDragDropCallBack(this, (DRAG_DROP_CALLBACK)&HierarchyGUI::DragDrop);

	//// 오브젝트 추가
	//for (UINT i = 0; i < MAX_SIZE_LAYER; ++i) {
	//	CLayer* pLayer = pCurScene->GetLayer(i);

	//	const vector<CGameObject*>& vecRootObjs = pLayer->GetRootGameObjects();
	//	const vector<CGameObject*>& vecRenderObj = pLayer->GetGameObjects();


	//	// DFS를 이용한 순환
	//	list<std::pair<CGameObject*, TreeViewNode*>> stk; // stk : stack
	//	for (UINT j = 0; j < vecRootObjs.size(); ++j) {
	//		UINT vecSize = vecRootObjs.size();
	//		
	//		stk.push_back(std::make_pair(vecRootObjs[j], pTreeViewRoot));

	//		while (!stk.empty()) {
	//			CGameObject* pObj = stk.back().first;
	//			TreeViewNode* pNode = stk.back().second;
	//			stk.pop_back();

	//			string strObjName;
	//			TStringToString(pObj->GetName(), strObjName);
	//			pNode = m_treeView.AddItem(strObjName, (DWORD_PTR)pObj, pNode);

	//			const vector<CGameObject*>& vecChilds = pObj->GetChildsObject();
	//			for (UINT k = 0; k < vecChilds.size(); ++k)
	//				stk.push_back(std::make_pair(vecChilds[k],pNode));
	//		}
	//	}
	//}
}

void HierarchyGUI::DragDrop(TreeViewNode* _pDragStartNode, TreeViewNode* _pDropTargetNode)
{
	CGameObject* pDragStartNode = (CGameObject*)_pDragStartNode->GetData();
	CGameObject* pDropTargetNode = nullptr;

	if (nullptr != _pDropTargetNode) {
		pDropTargetNode = (CGameObject*)_pDropTargetNode->GetData();
	}

	if (pDragStartNode == pDropTargetNode)
		return;

	if (nullptr == pDropTargetNode) {
		if (pDragStartNode->GetParentObject())
			CObject::UnlinkParentGameObjectEvn(pDragStartNode);
	}
	else {
		CObject::AddChildGameObjectEvn(pDropTargetNode, pDragStartNode);
	}
}

void HierarchyGUI::_ClickedGameObject(DWORD_PTR _dwItem, DWORD_PTR _dwData)
{
	TreeViewNode* pSelectedItem = (TreeViewNode*)_dwItem;
	CGameObject* pSelectedObj = (CGameObject*)_dwData;

	// InspectorGUI 에 해당 오브젝트의 정보를 요청한다.
	InspectorGUI* pInspector = (InspectorGUI*)CImGuiManager::GetInstance()->FindGUI(STR_GUI_Inspector);
	pInspector->SetTargetObject(pSelectedObj);

	// Hierachy 에 선택된 아이템 해제
	ResourceViewGUI* pResView = (ResourceViewGUI*)CImGuiManager::GetInstance()->FindGUI(STR_GUI_ResourceView);
	pResView->ReleaseSelectNode();
}
