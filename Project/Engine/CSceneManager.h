#pragma once

class CScene;
class CScript;
class CGameObject;

typedef CScript* (*pLoadScript)(FILE*);
typedef bool(*pSaveScript)(CScript*, FILE*);

class CSceneManager : public CSingleton<CSceneManager>
{
	SINGLETON(CSceneManager)

private:
	CScene* m_pCurScene;
	E_SceneMode m_eSceneMode;

public:
	// 스크립트 컴포넌트 Save, Load를 위한 함수 포인터
	pLoadScript m_pLoadScript;
	pSaveScript m_pSaveScript;

public:
	void Init();
	void Progress();
	void Render();

public:
	CScene* GetCurScene() { return m_pCurScene; }
	void ChangeScene(CScene* _pNextScene);

	CGameObject* FindGameObject(const tstring& _strName, E_Layer _eLayer = E_Layer::End);
	E_SceneMode GetSceneMode() { return m_eSceneMode; }

	void ChangeSceneModeEvt(E_SceneMode _eSceneMode); // 현재 씬의 모드 변경

private:
	void _SetSceneMode(E_SceneMode _eSceneMode) { m_eSceneMode = _eSceneMode; }
	friend class CEventManager;
};