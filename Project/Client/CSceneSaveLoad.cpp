#include "pch.h"
#include "CSceneSaveLoad.h"

#include <Engine\CPathManager.h>
#include <Engine\CScene.h>
#include <Engine\CSceneManager.h>

#include <Engine\CScript.h>
#include <Script\CScriptMgr.h>

#include <process.h>

HANDLE CSceneSaveLoad::g_hLoadThread = nullptr;
volatile int CSceneSaveLoad::g_iThreadDead = 0;
HANDLE CSceneSaveLoad::g_hLoadStartEvent = nullptr;
HANDLE CSceneSaveLoad::g_hLoadExitEvent = nullptr;
tstring CSceneSaveLoad::g_strRelativePath = _T("");

// TODO (Jang) : 까먹었다.. 다시 보기
UINT WINAPI Load_Thread(LPVOID _lparam) {
	CScene* &pNextScene = *((CScene**)_lparam);

	while (true) {
		// WaitForSingleObject함수를 이용하여 이벤트가 활성화 되었는지 비활성화 되었는지 알림을 받음
		if (WAIT_OBJECT_0 != WaitForSingleObject(CSceneSaveLoad::g_hLoadStartEvent, 0))
			continue;
		
		if (CSceneSaveLoad::g_iThreadDead) // 죽었으면 종료
			break;

		ResetEvent(CSceneSaveLoad::g_hLoadStartEvent);

		CScene* pScene = new CScene;

		tstring strFilePath = CPathManager::GetInstance()->GetContentPath();
		strFilePath += CSceneSaveLoad::g_strRelativePath;

		FILE* pFile = nullptr;
		errno_t err;
		err = _tfopen_s(&pFile, strFilePath.c_str(), _T("rb"));
		if (err) {
			if(pFile)
				fclose(pFile);
			return 1;
		}
		assert(pFile);

		pScene->LoadFromScene(pFile);

		fclose(pFile);

		// 시간 지연
		//Sleep(2000);

		pNextScene = pScene;
		CSceneManager::GetInstance()->m_iLoadSync = 1;
	}
	SetEvent(CSceneSaveLoad::g_hLoadExitEvent);
	_endthreadex(0);
	return 0;
}



void CSceneSaveLoad::Init()
{
	g_hLoadThread = (HANDLE)_beginthreadex(nullptr, 0, Load_Thread, &CSceneManager::GetInstance()->m_pNextScene, 0, nullptr);
	g_hLoadStartEvent = CreateEvent(nullptr, 0, 0, L"Thread Continue Event");
	g_hLoadExitEvent = CreateEvent(nullptr, 0, 0, L"Thread Exit Event");

	CSceneManager::GetInstance()->m_pSaveScript = &CSceneSaveLoad::SaveScript;
	CSceneManager::GetInstance()->m_pLoadScript = &CSceneSaveLoad::LoadScript;
}

void CSceneSaveLoad::Release()
{
	g_iThreadDead = 1;
	SetEvent(g_hLoadStartEvent);
	CloseHandle(g_hLoadThread);

	// WaitForSingleObject함수를 이용하여 이벤트가 활성화 되었는지 비활성화 되었는지 알림을 받음
	while (WAIT_OBJECT_0 != WaitForSingleObject(g_hLoadExitEvent, 0)) {	}
}

bool CSceneSaveLoad::SaveScene(CScene* _pScene, const tstring& _strPath, bool _bIsRelativePath)
{
	errno_t err;
	tstring strFilePath;
	if (_bIsRelativePath) {
		strFilePath = CPathManager::GetInstance()->GetContentPath();
		strFilePath += _strPath;
	}
	else
		strFilePath = _strPath;

	FILE* pFile = nullptr;
	err = _tfopen_s(&pFile, strFilePath.c_str(), _T("wb"));
	assert(pFile);
	if (err)
		return false;

	_pScene->SaveToScene(pFile);

	err = fclose(pFile);
	if (err) {
		assert(nullptr);
		return false;
	}

	return true;
}

CScene* CSceneSaveLoad::LoadScene(const tstring& _strPath, bool _bIsRelativePath)
{
	errno_t err;
	tstring strFilePath;
	if (_bIsRelativePath) {
		strFilePath = CPathManager::GetInstance()->GetContentPath();
		strFilePath += _strPath;
	}
	else
		strFilePath = _strPath;

	FILE* pFile = nullptr;
	err = _tfopen_s(&pFile, strFilePath.c_str(), _T("rb"));
	assert(pFile);
	if (err)
		return nullptr;

	CScene* pScene = new CScene;
	pScene->LoadFromScene(pFile);
	
	err = fclose(pFile);
	if (err) {
		assert(nullptr);
		return nullptr;
	}

	return pScene;
}

bool CSceneSaveLoad::SaveScript(CScript* _pScript, FILE* _pFile)
{
	tstring strScriptName = CScriptMgr::GetScriptName(_pScript);
	SaveStringToFile(strScriptName, _pFile);
	_pScript->SaveToScene(_pFile);
	return true;
}

CScript* CSceneSaveLoad::LoadScript(FILE* _pFile)
{
	tstring strScriptName;
	LoadStringFromFile(strScriptName, _pFile);

	CScript* pScript = CScriptMgr::GetScript(strScriptName);
	if (nullptr == pScript)
		return nullptr;
	pScript->LoadFromScene(_pFile);
	return pScript;
}

bool CSceneSaveLoad::TestSave()
{
	tstring strFilePath = _T("N:/testFile.test");

	struct TestDataType {
		int a;
		float b;
	};
	UINT a;
	tstring str;
	TestDataType d;

	d.a = 3;
	d.b = 3.3f;
	a = 9;
	str = _T("Test");

	errno_t err;

	FILE* pFile = nullptr;
	err = _tfopen_s(&pFile, strFilePath.c_str(), _T("wb"));
	if (err)
		assert(nullptr);

	FWrite(d, pFile);
	FWrite(a, pFile);
	SaveStringToFile(str, pFile);

	err = fclose(pFile);
	if (err)
		assert(nullptr);

	return true;
}

bool CSceneSaveLoad::TestLoad()
{
	tstring strFilePath = _T("N:/testFile.test");
	struct TestDataType {
		int a;
		float b;
	};
	UINT a;
	tstring str;
	TestDataType d;

	errno_t err;

	FILE* pFile = nullptr;
	err = _tfopen_s(&pFile, strFilePath.c_str(), _T("rb"));
	if (err)
		assert(nullptr);

	FRead(d, pFile);
	FRead(a, pFile);
	LoadStringFromFile(str, pFile);	

	err = fclose(pFile);
	if (err)
		assert(nullptr);

	return true;
}