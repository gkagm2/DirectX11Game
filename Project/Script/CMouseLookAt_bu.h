#pragma once
#include <Engine\CScript.h>
class CMouseLookAt_bu : public CScript
{
public:
	virtual void Update() override;

public:
	CLONE(CMouseLookAt_bu);
	CMouseLookAt_bu();
	virtual ~CMouseLookAt_bu() override {}
};