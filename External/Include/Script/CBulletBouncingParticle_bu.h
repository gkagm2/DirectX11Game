#pragma once
#include <Engine\CScript.h>
class CBulletBouncingParticle_bu : public CScript
{
private:
	float m_fSpeed;
	Vector3 m_vBouncingDir;
	float m_fMaxLifeTime;
	float m_fLifeTime;

public:
	virtual void Start() override;
	virtual void Update() override;

	virtual void OnCollisionEnter2D(CCollider2D* _pCol) override;

public:
	void SetParticle(const Vector3& _vBouncingDir);

public:
	CLONE(CBulletBouncingParticle_bu);
	CBulletBouncingParticle_bu();
	virtual ~CBulletBouncingParticle_bu() override {}
};