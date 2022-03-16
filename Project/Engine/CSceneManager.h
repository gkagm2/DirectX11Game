#pragma once
// FIXED (Jang) : 빌드 시 오래 걸림. 이게 없으면 CGameObject의 GetComponent에서 Script Type을 알아내지 못함.
#include "CScript.h" 
class CScript;

class CScene;
class CGameObject;

typedef CScript* (*pLoadScript)(FILE*);
typedef bool(*pSaveScript)(CScript*, FILE*);

typedef CScene* (*pLoadScene)(const tstring&,bool);
typedef bool(*pSAVE_SCENE)(CScene* _pScene, const tstring& _strPath, bool _bIsRelativePath);

class CSceneManager : public CSingleton<CSceneManager>
{
	SINGLETON(CSceneManager)

private:
	CScene* m_pCurScene;
	CScene* m_pLoadingScene;  // 로딩화면을 위한 씬
	E_SceneMode m_eSceneMode;

public:
	// 스크립트 컴포넌트 Save, Load를 위한 함수 포인터
	pLoadScript m_pLoadScript;
	pSaveScript m_pSaveScript;

	// Scene Load를 위한 함수 포인터
	pLoadScene m_pLoadSceneFunc;

	// Scene Save를 위한 함수 포인터
	pSAVE_SCENE m_pSaveSceneFunc;

public:
	void Init();
	void Progress();
	void Render();

public:
	CScene* GetCurScene() { return m_pCurScene; }
	

	CGameObject* FindGameObject(const tstring& _strName, UINT _iLayer = MAX_SIZE_LAYER);
	E_SceneMode GetSceneMode() { return m_eSceneMode; }

	void ChangeSceneModeEvt(E_SceneMode _eSceneMode); // 현재 씬의 모드 변경
	void ChangeSceneEvt(CScene* _pNextScene);
	void ChangeSceneEvt(const tstring& _pNextScenePath, bool _bRelativePath = true);
	void ChangeScene(CScene* _pNextScene);

	// For Multi Thread
public:
	volatile int m_iLoadSync;   // Scene Load가 끝나면 0에서 1로 설정됨
	CScene* m_pNextScene;

public:
	void SceneLoadStart();		//

public:
	CScene* CreateNewScene(); // 새로운 씬 생성 후 새 씬으로 이동


private:
	void _SetSceneMode(E_SceneMode _eSceneMode);
	friend class CEventManager;
};