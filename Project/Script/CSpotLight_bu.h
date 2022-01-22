#pragma once
#include <Engine\CScript.h>
class CSpotLight_bu : public CScript
{
public:
	virtual void OnCollisionEnter2D(CCollider2D* _pCol);
public:
	CLONE(CSpotLight_bu);
	CSpotLight_bu();
	virtual ~CSpotLight_bu() override {}
};