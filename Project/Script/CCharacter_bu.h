#pragma once
#include <Engine\CScript.h>


class CCharacter_bu : public CScript
{
public:
	float m_fJumpPower;
	float m_fMovePower;

	Vector3 m_vCurMoveDir;
public:
	virtual void Interaction() {}
	virtual void Attack() {}
	virtual void Jump() {}
	virtual void Move() {}


public:
	CLONE(CCharacter_bu);
	CCharacter_bu();
	CCharacter_bu(UINT _iScriptType);
	virtual ~CCharacter_bu() override;
};