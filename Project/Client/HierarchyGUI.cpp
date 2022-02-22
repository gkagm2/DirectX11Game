#include "pch.h"
#include "HierarchyGUI.h"
#include "TreeViewGUI.h"

#include <Engine\CEventManager.h>
#include <Engine\CSceneManager.h>
#include <Engine\CKeyManager.h>
#include <Engine\CScene.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\CRenderManager.h>
#include <Engine\CTransform.h>

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

	// HierarchyGUI �� ��Ŀ�� ���� ��, Ű �̺�Ʈ ó��
	if (ImGui::IsWindowFocused())
		_KeyCheck();

	ImGui::End();
}

void HierarchyGUI::_RenewTreeView()
{
	m_treeView.Clear();
	TreeViewNode* pTreeViewRoot = m_treeView.AddItem("RootHierachy", 0, nullptr);

	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();
	
	//  ������ �ֻ��� ������Ʈ���� ������
	vector<CGameObject*> vecRootObjs;
	pCurScene->GetRootGameObjects(vecRootObjs);

	// DFS�� �̿��� ��ȯ
	list<std::pair<CGameObject*, TreeViewNode*>> stk; // stk : stack
	for (int j = 0; j < vecRootObjs.size(); ++j) {
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
			for (int k = (int)vecChilds.size() - 1; k >= 0; --k)
				stk.push_back(std::make_pair(vecChilds[k], pNode));
		}
	}

	m_treeView.SetClickCallBack(this, TREE_CALLBACK(&HierarchyGUI::_ClickedGameObject));
	m_treeView.SetDragDropCallBack(this, (DRAG_DROP_CALLBACK)&HierarchyGUI::DragDrop);

	//// ������Ʈ �߰�
	//for (UINT i = 0; i < MAX_SIZE_LAYER; ++i) {
	//	CLayer* pLayer = pCurScene->GetLayer(i);

	//	const vector<CGameObject*>& vecRootObjs = pLayer->GetRootGameObjects();
	//	const vector<CGameObject*>& vecRenderObj = pLayer->GetGameObjects();


	//	// DFS�� �̿��� ��ȯ
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

	// InspectorGUI �� �ش� ������Ʈ�� ������ ��û�Ѵ�.
	InspectorGUI* pInspector = (InspectorGUI*)CImGuiManager::GetInstance()->FindGUI(STR_GUI_Inspector);
	pInspector->SetTargetObject(pSelectedObj);

	// Hierachy �� ���õ� ������ ����
	ResourceViewGUI* pResView = (ResourceViewGUI*)CImGuiManager::GetInstance()->FindGUI(STR_GUI_ResourceView);
	pResView->ReleaseSelectNode();
}

void HierarchyGUI::_KeyCheck()
{
	bool bDelete = false;
	if (InputKeyPress(E_Key::Delete))
		bDelete = true;

	bool bFocus = false;
	if (InputKeyPress(E_Key::F))
		bFocus = true;

	if (bDelete) {
		TreeViewNode* pSelItem = m_treeView._GetSelectdItem();
		if (!pSelItem)
			return;

		CGameObject* pObj = (CGameObject*)pSelItem->GetData();
		if (pObj)
			CObject::DestroyGameObjectEvn(pObj);
	}

	if (bFocus) {
		TreeViewNode* pSelItem = m_treeView._GetSelectdItem();
		if (!pSelItem)
			return;
		CGameObject* pObj = (CGameObject*)pSelItem->GetData();
		if (pObj) {
			CCamera* pToolCam = CRenderManager::GetInstance()->GetToolCamera();
			if (pToolCam) {
				if (E_ProjectionType::Perspective == pToolCam->GetProjectionType()) {
					Vector3 vObjPos = pObj->Transform()->GetPosition();
					Vector3 vObjScale = pObj->Transform()->GetScale();
					Vector3 vNewCamPos = vObjPos;
					float fMaxScale = max(vObjScale.x, max(vObjScale.y, vObjScale.z));
					vNewCamPos.z += fMaxScale;
					
					pObj->Transform()->GetPosition();
					pToolCam->Transform()->SetLocalPosition(vNewCamPos);
					pToolCam->Transform()->LookAt3D(vObjPos);
				}
			}
		}
	}
}