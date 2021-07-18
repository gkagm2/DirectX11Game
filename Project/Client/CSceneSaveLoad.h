#pragma once
class CScene;
class CSceneSaveLoad
{
public:
	static bool SaveScene(CScene* _pScene, const tstring& _strRelativePath);
	static CScene* LoadScene(const tstring& _strRelativePath);

	static bool TestSave();
	static bool TestLoad();

};