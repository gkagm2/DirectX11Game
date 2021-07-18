#include "pch.h"
#include "CSceneSaveLoad.h"

#include <Engine\CPathManager.h>
#include <Engine\CScene.h>
#include <Engine\CSceneManager.h>

#include <Engine\CScript.h>
#include <Script\CScriptMgr.h>

void CSceneSaveLoad::Init()
{
	CSceneManager::GetInstance()->m_pSaveScript = &CSceneSaveLoad::SaveScript;

	CSceneManager::GetInstance()->m_pLoadScript = &CSceneSaveLoad::LoadScript;
}

bool CSceneSaveLoad::SaveScene(CScene* _pScene, const tstring& _strRelativePath)
{
	errno_t err;
	tstring strFilePath = CPathManager::GetInstance()->GetContentPath();
	strFilePath += _strRelativePath;

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

CScene* CSceneSaveLoad::LoadScene(const tstring& _strRelativePath)
{
	errno_t err;
	tstring strFilePath = CPathManager::GetInstance()->GetContentPath();
	strFilePath += _strRelativePath;

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