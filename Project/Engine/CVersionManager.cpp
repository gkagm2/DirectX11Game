#include "pch.h"
#include "CVersionManager.h"
#include "CCore.h"
#include "CKeyManager.h"
#include "CResourceManager.h"
#include "CSceneManager.h"
#include "CEventManager.h"
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
	m_vecSceneFileNames = CPathManager::GetInstance()->GetFilesInDirectory(strContentPath, _T("*.scene"));
}

void CVersionManager::Update()
{
	if (!g_bOldVersionUpdate)
		return;

	// TODO (Jang) : Reupdate하기 전 backup기능 만들기

	// load되어있는 모든 프리펩스를 전부 세이브
	if (InputKeyPress(E_Key::P)) {
		CResourceManager::GetInstance()->RenewResourcesFromDir(E_ResourceType::Prefab);
	}
	if (InputKeyPress(E_Key::O)) {
		// 모든 씬을 순회로 업데이트
		ReupdateAllSceneEvt();
	}
}

void CVersionManager::ReupdateAllSceneEvt()
{
	for (int i = 0; i < m_vecSceneFileNames.size(); ++i) {
		TEvent evn{};
		evn.eType = E_EventType::Reupdate_All_Scene;
		tstring* pStrFileName = new tstring(m_vecSceneFileNames[i]);
		evn.lparam = (DWORD_PTR)pStrFileName;
		CEventManager::GetInstance()->AddEvent(evn);
	}
}
