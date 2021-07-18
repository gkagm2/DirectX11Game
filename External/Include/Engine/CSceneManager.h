#pragma once

class CScene;
class CScript;

typedef CScript* (*pLoadScript)(FILE*);
typedef bool(*pSaveScript)(CScript*, FILE*);

class CSceneManager : public CSingleton<CSceneManager>
{
	SINGLETON(CSceneManager)

private:
	CScene* m_pCurScene;

public:
	// ��ũ��Ʈ ������Ʈ Save, Load�� ���� �Լ� ������
	pLoadScript m_pLoadScript;
	pSaveScript m_pSaveScript;

public:
	void Init();
	void Progress();
	void Render();

public:
	void CreateScene(const tstring& _strSceneName);
	void LoadScene(const tstring& _strSceneName);

public:
	CScene* GetCurScene() { return m_pCurScene; }
	void ChangeScene(CScene* _pNextScene);

	friend class CScene;
};