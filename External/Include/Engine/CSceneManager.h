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
	CScene* m_pLoadingScene;  // �ε�ȭ���� ���� ��
	E_SceneMode m_eSceneMode;

public:
	// ��ũ��Ʈ ������Ʈ Save, Load�� ���� �Լ� ������
	pLoadScript m_pLoadScript;
	pSaveScript m_pSaveScript;

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
	void ChangeScene(CScene* _pNextScene);

	// For Multi Thread
public:
	volatile int m_iLoadSync;   // Scene Load�� ������ 0���� 1�� ������
	CScene* m_pNextScene;

public:
	void SceneLoadStart();		//


private:
	void _SetSceneMode(E_SceneMode _eSceneMode);
	friend class CEventManager;
};