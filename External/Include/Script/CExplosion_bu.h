#pragma once
#include <Engine\CScript.h>
class CExplosion_bu : public CScript
{
private:
	Vector3 m_vExplosionDir;
	float m_fForce;
	float m_fForceTime;
	float m_fMaxForceTime;

	float m_fDelTime;
	float m_fTouchTime;

public:
	virtual void Awake() override;
	virtual void Update() override;

	void SetExplosion(const Vector3& _vExplosionDir, float _fForce, float _fForceTime);

	virtual void OnCollisionStay2D(CCollider2D* _pCol) override;
public:
	CLONE(CExplosion_bu);
	CExplosion_bu();
	CExplosion_bu(const CExplosion_bu& _origin);
	virtual ~CExplosion_bu() override{}
};