#include "pch.h"
#include "CVersionManager.h"
#include "CCore.h"
#include "CKeyManager.h"
#include "CResourceManager.h"
#include "CSceneManager.h"
#include "CEventManager.h"
bool CVersionManager::g_bOldVersionUpdate = false;	 // ���Ӱ� ������ �߰����� ��쿡�� ���
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

	// ��� ������ �ε�
	//CResourceManager::GetInstance()->LoadResourcesFromDir<CPrefab>(_T("prefab\\"), _T("*.pref"));


	// ��� ���� �����´�.
	tstring strContentPath = CPathManager::GetInstance()->GetContentPath();
	strContentPath += STR_DIR_PATH_Scene;
	m_vecSceneFileNames = CPathManager::GetInstance()->GetFilesInDirectory(strContentPath, _T("*.scene"));
}

void CVersionManager::Update()
{
	if (!g_bOldVersionUpdate)
		return;

	// TODO (Jang) : Reupdate�ϱ� �� backup��� �����

	// load�Ǿ��ִ� ��� �����齺�� ���� ���̺�
	if (InputKeyPress(E_Key::P)) {
		CResourceManager::GetInstance()->RenewResourcesFromDir(E_ResourceType::Prefab);
	}
	if (InputKeyPress(E_Key::O)) {
		// ��� ���� ��ȸ�� ������Ʈ
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
