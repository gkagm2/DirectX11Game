#pragma once
#include "CScript.h"
class CPlayerScript_sh : public CScript
{
private:
	float m_fHp;
	float m_fBulletDamage;
	float m_fMoveSpeed;

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void Shoot();
	void Move();

public:
	float GetHp() { return m_fHp; }
	void SetHp(float _iHp) { m_fHp = _iHp; }

	int GetBulletDamage() { return m_fBulletDamage; }
	void SetBulletDamage(float _iDamage) { m_fBulletDamage = _iDamage; }

public:
	CPlayerScript_sh();
	virtual ~CPlayerScript_sh() override;
};