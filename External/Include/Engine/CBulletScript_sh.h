#pragma once
#include "CScript.h"

enum class E_BulletType_sh {
	Straight,
	Spread
};

class CBulletScript_sh : public CScript
{
private:
	float m_fSpeed;
	Vector3 m_vDir;

	E_BulletType_sh m_eType;

	float m_fDeleteMaxTime;
	float m_fDeleteCoolTime;

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void SetDirection(const Vector3& _vDir) { m_vDir = _vDir; }
	void SetBulletType(E_BulletType_sh _eType) { m_eType = _eType; }
	void SetBulletSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }

public:
	CBulletScript_sh();
	virtual ~CBulletScript_sh() override;
};