#pragma once
#include <Engine\CScript.h>

class CCharacter_bu : public CScript
{
protected:
	float m_fJumpPower;
	float m_fMovePower;
	Vector3 m_vCurMoveDir;

	CGameObject* m_pMuzzlObj;

	float m_fHp;
	float m_fArmor;

public:
	virtual void Interaction() {}
	virtual void Attack() {}
	virtual void Jump() {}
	virtual void Move() {}
	
	virtual void DamagedMe(float _fDamage);

	virtual void OnDead(); // ���� ����

public:
	float GetHp() { return m_fHp; }
	float GetArmor() { return m_fArmor; }
	float SetHp(float _fHp) { m_fHp = _fHp; }
	float SetArmor(float _fArmor) { m_fArmor = _fArmor; }

public:
	CLONE(CCharacter_bu);
	CCharacter_bu();
	CCharacter_bu(UINT _iScriptType);
	virtual ~CCharacter_bu() override;
};