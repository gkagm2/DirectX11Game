#pragma once
class CScene;
class CScript;
class CSceneSaveLoad
{
public:
	static HANDLE g_hLoadThread;
	static volatile int g_iThreadDead; // 0 : not dead, 1 dead

	static HANDLE g_hLoadStartEvent;
	static HANDLE g_hLoadExitEvent;

	static tstring g_strRelativePath;

public:
	static void Init();
	static void Release();

	static bool SaveScene(CScene* _pScene, const tstring& _strRelativePath);
	static CScene* LoadScene(const tstring& _strRelativePath);

	static bool SaveScript(CScript* _pScript, FILE* _pFile);
	static CScript* LoadScript(FILE* _pFile);

	static bool TestSave();
	static bool TestLoad();

};