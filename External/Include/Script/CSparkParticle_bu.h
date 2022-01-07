#pragma once
#include <Engine\CScript.h>
class CSparkParticle_bu : public CScript
{
private:
	CParticleSystem* m_pPS;
	float m_fLifeTime;
	float m_fMaxLifeTime;
	
public:
	virtual void Awake() override;
	virtual void Update() override;

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CSparkParticle_bu);
	CSparkParticle_bu();
	CSparkParticle_bu(const CSparkParticle_bu& _origin);
	virtual ~CSparkParticle_bu() override;
};