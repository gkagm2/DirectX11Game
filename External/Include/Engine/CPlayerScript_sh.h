#pragma once
#include "CScript.h"
class CPlayerScript_sh : public CScript
{
private:
	float m_fHp;
	float m_fBulletDamage;
	float m_fMoveSpeed;

	SharedPtr<CMaterial> m_pSharedMtrl;
	SharedPtr<CMaterial> m_pCloneMtrl;

	SharedPtr<CPrefab> m_pMissilePrefab;

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void Shoot(const Vector2& _vTargetPos);
	void Move();

public:
	float GetHp() { return m_fHp; }
	void SetHp(float _iHp) { m_fHp = _iHp; }

	float GetBulletDamage() { return m_fBulletDamage; }
	void SetBulletDamage(float _iDamage) { m_fBulletDamage = _iDamage; }

public:
	CLONE(CPlayerScript_sh);
	CPlayerScript_sh();
	virtual ~CPlayerScript_sh() override;
};