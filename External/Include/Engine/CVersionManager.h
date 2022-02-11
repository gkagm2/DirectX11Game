#pragma once
#include "CSingleton.h"
class CVersionManager : public CSingleton<CVersionManager>
{
private:
	SINGLETON(CVersionManager);

private:
	vector<tstring> m_vecSceneFileNames;
	int iProgressScene;
public:
	static bool g_bOldVersionUpdate;	 // 새롭게 버전을 추가했을 경우에만 사용

	static bool g_bComponentUpdate; // Component를 추가 한경우 true로 바꿔줌.
public:
	bool IsComponentUpdate() {
		return g_bOldVersionUpdate && g_bComponentUpdate;
	}

public:
	void Init();
	void Update();

	void ReupdateAllSceneEvt();
};