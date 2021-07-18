#pragma once
class CScene;
class CScript;
class CSceneSaveLoad
{
public:
	static void Init();
	static bool SaveScene(CScene* _pScene, const tstring& _strRelativePath);
	static CScene* LoadScene(const tstring& _strRelativePath);

	static bool SaveScript(CScript* _pScript, FILE* _pFile);
	static CScript* LoadScript(FILE* _pFile);

	static bool TestSave();
	static bool TestLoad();

};