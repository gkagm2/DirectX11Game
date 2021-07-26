#include "pch.h"
#include "HierachyGUI.h"
#include "TreeViewGUI.h"

#include <Engine\CEventManager.h>
#include <Engine\CSceneManager.h>
#include <Engine\CScene.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>

HierachyGUI::HierachyGUI()
{
	SetName("Hierachy");
}

HierachyGUI::~HierachyGUI()
{
}

void HierachyGUI::Init()
{
	_RenewTreeView();
	m_treeView.SetFrameRender(false);
	m_treeView.SetFrameOnlyParent(false);
}

void HierachyGUI::Update()
{
	if (CEventManager::GetInstance()->DidEventHappended())
		_RenewTreeView();

	ImGui::Begin(GetName().c_str(), &m_bGUIOpen);
	m_treeView.Update();
	ImGui::End();
}

void HierachyGUI::_RenewTreeView()
{
	m_treeView.Clear();
	TreeViewNode* pTreeViewRoot = m_treeView.AddItem("RootHierachy", 0, nullptr);

	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();

	// ������Ʈ �߰�
	for (UINT i = 0; i < (UINT)E_Layer::End; ++i) {
		CLayer* pLayer = pCurScene->GetLayer(i);

		const vector<CGameObject*>& vecRootObjs = pLayer->GetRootGameObjects();

		// DFS�� �̿��� ��ȯ
		list<CGameObject*> stk; // stk : stack
		for (UINT j = 0; j < vecRootObjs.size(); ++j)
			stk.push_back(vecRootObjs[j]);
		
		while (!stk.empty()) {
			CGameObject* pObj = stk.back();
			stk.pop_back();
			string strObjName;
			TStringToString(pObj->GetName(), strObjName);
			TreeViewNode* pNode = m_treeView.AddItem(strObjName, (DWORD_PTR)pObj, pTreeViewRoot);

			const vector<CGameObject*>& vecChilds = pObj->GetChildsObject();
			for (UINT k = 0; k < vecChilds.size(); ++k)
				stk.push_back(vecRootObjs[k]);
		}
	}
}