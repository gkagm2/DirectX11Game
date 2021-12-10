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

HierarchyGUI::HierarchyGUI()
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
	m_treeView.SetSelectCallBack(this, (SEL_CHANGE_CALLBACK)&HierarchyGUI::SelectGameObject);
	m_treeView.SetDragDropCallBack(this, (DRAG_DROP_CALLBACK)&HierarchyGUI::DragDrop);
}

void HierarchyGUI::Update()
{
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

			if (pObj->IsActive())
				pNode->SetTextColor(ImVec4(1.f, 1.f, 1.f, 1.f));
			else
				pNode->SetTextColor(ImVec4(0.4f, 0.4f, 0.4f, 1.f));

			const vector<CGameObject*>& vecChilds = pObj->GetChildsObject();
			for (UINT k = 0; k < vecChilds.size(); ++k)
				stk.push_back(std::make_pair(vecChilds[k], pNode));
		}
	}

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

void HierarchyGUI::SelectGameObject(TreeViewNode* _pNode)
{
	CGameObject* pTargetObj = (CGameObject*)_pNode->GetData();
	InspectorGUI* pInspectorGUI = (InspectorGUI*)CImGuiManager::GetInstance()->FindGUI(STR_GUI_Inspector);
	
	pInspectorGUI->SetTargetObject(pTargetObj);
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