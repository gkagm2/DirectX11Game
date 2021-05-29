#pragma once
class CSceneManager : public CSingleton<CSceneManager>
{
	SINGLETON(CSceneManager)

public:
	void CreateScene(const tstring& _strSceneName);
	void LoadScene(const tstring& _strSceneName);

	friend class CScene;
};

