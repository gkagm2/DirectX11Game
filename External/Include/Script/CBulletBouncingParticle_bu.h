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

public:
	// fSideDegree : ªÁ¿’∞¢
	void SetParticle(const Vector3& _vBouncingDir, float _fSideDegree);

public:
	CLONE(CBulletBouncingParticle_bu);
	CBulletBouncingParticle_bu();
	virtual ~CBulletBouncingParticle_bu() override {}
};