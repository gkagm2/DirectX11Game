#pragma once
// FIXED (Jang) : ���� �� ���� �ɸ�. �̰� ������ CGameObject�� GetComponent���� Script Type�� �˾Ƴ��� ����.
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
	CScene* m_pLoadingScene;  // �ε�ȭ���� ���� ��
	E_SceneMode m_eSceneMode;

public:
	// ��ũ��Ʈ ������Ʈ Save, Load�� ���� �Լ� ������
	pLoadScript m_pLoadScript;
	pSaveScript m_pSaveScript;

	// Scene Load�� ���� �Լ� ������
	pLoadScene m_pLoadSceneFunc;

	// Scene Save�� ���� �Լ� ������
	pSAVE_SCENE m_pSaveSceneFunc;

public:
	void Init();
	void Progress();
	void Render();

public:
	CScene* GetCurScene() { return m_pCurScene; }
	

	CGameObject* FindGameObject(const tstring& _strName, UINT _iLayer = MAX_SIZE_LAYER);
	E_SceneMode GetSceneMode() { return m_eSceneMode; }

	void ChangeSceneModeEvt(E_SceneMode _eSceneMode); // ���� ���� ��� ����
	void ChangeSceneEvt(CScene* _pNextScene);
	void ChangeSceneEvt(const tstring& _pNextScenePath, bool _bRelativePath = true);
	void ChangeScene(CScene* _pNextScene);

	// For Multi Thread
public:
	volatile int m_iLoadSync;   // Scene Load�� ������ 0���� 1�� ������
	CScene* m_pNextScene;

public:
	void SceneLoadStart();		//

public:
	CScene* CreateNewScene(); // ���ο� �� ���� �� �� ������ �̵�


private:
	void _SetSceneMode(E_SceneMode _eSceneMode);
	friend class CEventManager;
};