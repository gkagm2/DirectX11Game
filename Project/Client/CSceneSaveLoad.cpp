#include "pch.h"
#include "CSceneSaveLoad.h"

#include <Engine\CPathManager.h>
#include <Engine\CScene.h>

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