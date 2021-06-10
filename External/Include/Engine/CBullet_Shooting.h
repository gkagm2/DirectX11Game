#pragma once
#include "CScript.h"
class CBullet_Shooting : public CScript
{
private:
	float m_fSpeed;
	Vector3 m_vDir;

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	CBullet_Shooting();
	virtual ~CBullet_Shooting() override;
};