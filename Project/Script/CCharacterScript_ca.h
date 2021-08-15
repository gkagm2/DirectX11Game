#pragma once
#include <Engine\CScript.h>
class CCharacterScript_ca : public CScript
{
public:
	virtual void Start() override;
	virtual void Update() override;
public:
	CLONE(CCharacterScript_ca);
	CCharacterScript_ca();
	virtual ~CCharacterScript_ca() override;
};