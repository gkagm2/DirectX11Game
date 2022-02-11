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
	static bool g_bOldVersionUpdate;	 // ���Ӱ� ������ �߰����� ��쿡�� ���

	static bool g_bComponentUpdate; // Component�� �߰� �Ѱ�� true�� �ٲ���.
public:
	bool IsComponentUpdate() {
		return g_bOldVersionUpdate && g_bComponentUpdate;
	}

public:
	void Init();
	void Update();

	void ReupdateAllSceneEvt();
};