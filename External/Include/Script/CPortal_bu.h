#pragma once
#include <Engine\CScript.h>
class CGameManager_bu;
class CPortal_bu : public CScript
{
private:
	CGameManager_bu* m_pGameMgr;

	CGameObject* m_pPlayerObj;
	float m_vOrignZ;
	CLight2D* m_pLight;

public:
	virtual void Awake() override;
	virtual void Update() override;

public:
	CLONE(CPortal_bu);
	CPortal_bu();
	virtual ~CPortal_bu() override {}
};