#include "pch.h"
#include "SceneViewGUI.h"
#include <Engine\CPathManager.h>
#include <Engine\CSceneManager.h>
#include <Engine\CEventManager.h>
#include "CSceneSaveLoad.h"

SceneViewGUI::SceneViewGUI()
{
	SetName(STR_GUI_SceneView);
}

SceneViewGUI::~SceneViewGUI()
{
}

void SceneViewGUI::Init()
{
	m_treeView.SetFrameRender(true);
	m_treeView.SetFrameOnlyParent(true);
	m_treeView.SetRootRender(false);

	m_treeView.SetDoubleClickCallBack(this, (GUI_CALLBACK)&SceneViewGUI::SelectScene);

	_RenewTreeView();
}

void SceneViewGUI::Update()
{
	_RenewScene();

	ImGui::Begin(GetName().c_str(), &m_bGUIOpen);
	m_treeView.Update();
	ImGui::End();
}

void SceneViewGUI::_RenewScene()
{
	if(CEventManager::GetInstance()->DidEventHappended())
		_RenewTreeView();
}

void SceneViewGUI::_RenewTreeView()
{
	m_treeView.Clear();


	TreeViewNode* pRoot = m_treeView.AddItem("RootScene", 0, nullptr);

	bool bUseFrame = true;

	// ���Ͽ��� �о ��� Scene ������ Vector�� �ִ´�.
	tstring strContentPath =CPathManager::GetInstance()->GetContentPath();
	strContentPath += STR_DIR_PATH_Scene;
	vector<tstring> vectFileNames;
	vectFileNames = CPathManager::GetInstance()->GetFilesInDirectory(strContentPath, _T("*.scene"));

	vector<string> vecFileNames;
	UINT fileCnt = vectFileNames.size();

	string strFileName;
	vecFileNames.resize(fileCnt);
	for (UINT i = 0; i < fileCnt; ++i)
		TStringToString(vectFileNames[i], vecFileNames[i]);

	// Vector�� ����ִ� ���� �̸����� Ʈ���並 �����Ѵ�.
	for (UINT i = 0; i < vecFileNames.size(); ++i) {
		TreeViewNode* pNode = m_treeView.AddItem(vecFileNames[i].c_str(), 0, pRoot);
	}
}

void SceneViewGUI::SelectScene(DWORD_PTR _strRelativePath, DWORD_PTR _non)
{
	string str = ((string*)_strRelativePath)->c_str();
	tstring tstrRelativePath;
	StringToTString(str, tstrRelativePath);

	CSceneManager::GetInstance()->ChangeSceneModeEvt(E_SceneMode::Stop);
	CScene* pNewScene = CSceneSaveLoad::LoadScene(tstrRelativePath, true);
	CSceneManager::GetInstance()->ChangeSceneEvt(pNewScene);
}