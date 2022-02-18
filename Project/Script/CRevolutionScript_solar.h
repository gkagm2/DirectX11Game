#pragma once
#include <Engine\CScript.h>
class CRevolutionScript_solar : public CScript
{
public:
	virtual void Update() override;

public:
	CLONE(CRevolutionScript_solar);
	CRevolutionScript_solar();
	virtual ~CRevolutionScript_solar() override;
};