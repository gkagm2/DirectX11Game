#pragma once
#include <Engine\CScript.h>
class CObjectManager_bu : public CScript
{
private:
	SharedPtr<CPrefab> m_pPlayerPref;
	SharedPtr<CPrefab> m_pEnemyPref;
	//SharedPtr<CPrefab> m_pDestObjPref;

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;

public:
	CLONE(CObjectManager_bu);
	CObjectManager_bu();
	virtual ~CObjectManager_bu() override;
};