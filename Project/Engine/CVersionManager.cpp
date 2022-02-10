#include "pch.h"
#include "CVersionManager.h"
#include "CCore.h"
#include "CKeyManager.h"
#include "CResourceManager.h"
#include "CSceneManager.h"
bool CVersionManager::g_bOldVersionUpdate = false;	 // 새롭게 버전을 추가했을 경우에만 사용
bool CVersionManager::g_bComponentUpdate = false;

CVersionManager::CVersionManager() : 
	iProgressScene(0)
{
}

CVersionManager::~CVersionManager()
{
}

void CVersionManager::Init()
{
	if (!g_bOldVersionUpdate)
		return;

	// 모든 프리펩 로딩
	//CResourceManager::GetInstance()->LoadResourcesFromDir<CPrefab>(_T("prefab\\"), _T("*.pref"));


	// 모든 씬을 가져온다.
	tstring strContentPath = CPathManager::GetInstance()->GetContentPath();
	strContentPath += STR_DIR_PATH_Scene;
	vector<tstring> vecSceneFileNames = CPathManager::GetInstance()->GetFilesInDirectory(strContentPath, _T("*.scene"));
}

void CVersionManager::Update()
{
	if (!g_bOldVersionUpdate)
		return;

	// load되어있는 모든 프리펩스를 전부 세이브
	if (InputKeyPress(E_Key::P)) {
		CResourceManager::GetInstance()->RenewResourcesFromDir(E_ResourceType::Prefab);
	}
	//if (InputKeyPress(E_Key::O)) {
	//	// 모든 씬을 순회로 업데이트 할 수 있도록 만든다.
	//	// TODO (Jang) : 스레드 이용해야될 것 같다.
	//		// Load
	//	for (int i = 0; i < vecSceneFileNames.size(); ++i) {
	//		CScene* pCurScene = CSceneSaveLoad::LoadScene(path, false);
	//		CSceneManager::GetInstance()->ChangeSceneEvt(pCurScene);
	//	}
	//}
}