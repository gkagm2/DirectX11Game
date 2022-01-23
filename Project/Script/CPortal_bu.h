#pragma once
#include <Engine\CScript.h>
class CPortal_bu : public CScript
{
private:
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