#pragma once
#include <Engine\CScript.h>
class CActivatorController;
class CDeactivator_bu : public CScript
{
public:	
	CActivatorController* m_pController;
	bool m_bIsDeactivatorActive;
public:
	virtual void OnCollisionEnter2D(CCollider2D* _pOther);
public:
	CLONE(CDeactivator_bu);
	CDeactivator_bu();
	virtual ~CDeactivator_bu() override {}
};