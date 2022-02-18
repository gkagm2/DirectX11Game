#pragma once
#include <Engine\CScript.h>
class CRotationSelfScript_solar : public CScript
{
public:
	virtual void Update() override;
public:
	CLONE(CRotationSelfScript_solar);
	CRotationSelfScript_solar();
	virtual ~CRotationSelfScript_solar() override;
};