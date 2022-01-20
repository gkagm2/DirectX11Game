#pragma once
#include <Engine\CScript.h>
class CExitDoorEndCol_bu : public CScript
{
public:
	virtual void OnCollisionEnter2D(CCollider2D* pCol) override;

public:
	CLONE(CExitDoorEndCol_bu);
	CExitDoorEndCol_bu();
	virtual ~CExitDoorEndCol_bu() override {}
};