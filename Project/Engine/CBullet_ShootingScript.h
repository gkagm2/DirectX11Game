#pragma once
#include "CScript.h"
class CBullet_ShootingScript : public CScript
{
private:
	float m_fSpeed;
	Vector3 m_vDir;

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	CBullet_ShootingScript();
	virtual ~CBullet_ShootingScript() override;
};