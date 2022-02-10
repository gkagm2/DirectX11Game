#include "pch.h"
#include "SceneViewGUI.h"
#include <Engine\CPathManager.h>
#include <Engine\CSceneManager.h>
#include <Engine\CEventManager.h>
#include "CSceneSaveLoad.h"
#include "InspectorGUI.h"
#include "CImGuiManager.h"
#include "MainMenuGUI.h"

SceneViewGUI::SceneViewGUI() :
	m_treeView(STR_GUI_SceneViewTree)
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
	_RenewTreeView();
}

void SceneViewGUI::Update()
{
	if (!m_bGUIOpen)
		return;

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

	vector<string> vecSceneFileNames;
	UINT fileCnt = (UINT)vectFileNames.size();

	string strFileName;
	vecSceneFileNames.resize(fileCnt);
	for (UINT i = 0; i < fileCnt; ++i)
		TStringToString(vectFileNames[i], vecSceneFileNames[i]);

	// Vector�� ����ִ� ���� �̸����� Ʈ���並 �����Ѵ�.
	for (UINT i = 0; i < vecSceneFileNames.size(); ++i) {
		TreeViewNode* pNode = m_treeView.AddItem(vecSceneFileNames[i].c_str(), 0, pRoot);
	}
	m_treeView.SetDoubleClickCallBack(this, (GUI_CALLBACK)&SceneViewGUI::SelectScene);
}

void SceneViewGUI::SelectScene(DWORD_PTR _strRelativePath, DWORD_PTR _non)
{
	string str = ((string*)_strRelativePath)->c_str();
	tstring tstrRelativePath;
	StringToTString(str, tstrRelativePath);

	
	MainMenuGUI* pMainMenuGUI = (MainMenuGUI*)CImGuiManager::GetInstance()->FindGUI(STR_GUI_MainMenu);
	if (pMainMenuGUI)
		pMainMenuGUI->_SetSceneModeMenu(E_SceneMode::Stop);


	CSceneManager::GetInstance()->ChangeSceneModeEvt(E_SceneMode::Stop);
	CScene* pNewScene = CSceneSaveLoad::LoadScene(tstrRelativePath, true);
	CSceneManager::GetInstance()->ChangeSceneEvt(pNewScene);
	InspectorGUI* pInspectorGUI = (InspectorGUI*)CImGuiManager::GetInstance()->FindGUI(STR_GUI_Inspector);
	pInspectorGUI->SetInspectorUIMode(E_InspectorUIMode::None);
}