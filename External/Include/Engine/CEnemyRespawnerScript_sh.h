#pragma once
#include "CScript.h"
class CEnemyRespawnerScript_sh : public CScript
{
private:
	Vector3 m_vRespawnPos;
	float m_fRespawnCoolTime;
	float m_fRespawnMaxCoolTime;
	

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;

public:
	void CreateEnemy();

public:
	CLONE(CEnemyRespawnerScript_sh);
	CEnemyRespawnerScript_sh();
	virtual ~CEnemyRespawnerScript_sh() override;
};