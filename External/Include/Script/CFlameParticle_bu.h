#pragma once
#include <Engine\CScript.h>
class CFlameParticle_bu : public CScript
{
private:
	CAnimator2D* m_pAnim;
public:
	virtual void Awake() override;
	virtual void Update() override;
public:
	CLONE(CFlameParticle_bu);
	CFlameParticle_bu();
	virtual ~CFlameParticle_bu() override {}
};