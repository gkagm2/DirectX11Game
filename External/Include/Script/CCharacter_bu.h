#pragma once
#include <Engine\CScript.h>
class CCharacter_bu : public CScript
{
public:
	float m_fJumpPower;
	float m_fMovPower;

public:

public:
	CLONE(CCharacter_bu);
	CCharacter_bu();
	CCharacter_bu(UINT _iScriptType);
	virtual ~CCharacter_bu() override;
};