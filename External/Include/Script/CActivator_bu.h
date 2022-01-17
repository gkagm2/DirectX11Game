#pragma once
#include <Engine\CScript.h>
class CActivatorController;
class CActivator_bu : public CScript
{
private:
	CActivatorController* m_pController;
	bool m_bIsActivatorActive;

public:
	virtual void Awake() override;
	virtual void OnCollisionEnter2D(CCollider2D* _pOther);

public:
	CLONE(CActivator_bu);
	CActivator_bu();
	virtual ~CActivator_bu() override{}
};