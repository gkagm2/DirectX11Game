#pragma once
#include <Engine\CScript.h>
class CMuzzleParticle_bu : public CScript
{
private:
	CAnimator2D* pMuzzleAnim;

	virtual void Update() override;
	virtual void OnEnable() override;
	
public:
	CLONE(CMuzzleParticle_bu);
	CMuzzleParticle_bu();
	virtual ~CMuzzleParticle_bu() override {}
};