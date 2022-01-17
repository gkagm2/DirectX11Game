#pragma once
#include <Engine\CScript.h>
class CRespawnManager_bu : public CScript
{
private:
	vector<CGameObject*> m_vecStages;
	SharedPtr<CPrefab> m_pPlayerPref;
	SharedPtr<CPrefab> m_pEnemyPref;
	SharedPtr<CPrefab> m_pDestObjPref;

public:
	virtual void Update() override;
	virtual void Awake() override;

	void CreateEnemey(const Vector3& _vPos, int _pInsertStageNum);
	void CreatePlayer(const Vector3& _vPos, int _pInsertStageNum);

public:
	CLONE(CRespawnManager_bu);
	CRespawnManager_bu();
	virtual ~CRespawnManager_bu() override {}
};