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
	// ��ũ��Ʈ ������Ʈ Save, Load�� ���� �Լ� ������
	pLoadScript m_pLoadScript;
	pSaveScript m_pSaveScript;

public:
	void Init();
	void Progress();
	void Render();

public:
	CScene* GetCurScene() { return m_pCurScene; }
	

	CGameObject* FindGameObject(const tstring& _strName, E_Layer _eLayer = E_Layer::End);
	E_SceneMode GetSceneMode() { return m_eSceneMode; }

	void ChangeSceneModeEvt(E_SceneMode _eSceneMode); // ���� ���� ��� ����
	void ChangeSceneEvt(CScene* _pNextScene);
	void ChangeScene(CScene* _pNextScene);

private:
	void _SetSceneMode(E_SceneMode _eSceneMode);
	friend class CEventManager;
};