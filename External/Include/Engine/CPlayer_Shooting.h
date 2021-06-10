#pragma once
#include "CScript.h"
class CPlayer_Shooting : public CScript
{
private:
	float m_fHp;
	float m_fBulletDamage;

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void Shoot();
	void Move();

public:
	int GetHp() { return m_fHp; }
	void SetHp(int _iHp) { m_fHp = _iHp; }

	int GetBulletDamage() { return m_fBulletDamage; }
	void SetBulletDamage(int _iDamage) { m_fBulletDamage = _iDamage; }

public:
	CPlayer_Shooting();
	virtual ~CPlayer_Shooting() override;
};