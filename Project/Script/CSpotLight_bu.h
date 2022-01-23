#pragma once
#include <Engine\CScript.h>
class CSpotLight_bu : public CScript
{
private:
	SharedPtr<CPrefab> m_pSparkParticle;
public:
	virtual void OnCollisionEnter2D(CCollider2D* _pCol);
	virtual void OnDestroy() override;
public:
	CLONE(CSpotLight_bu);
	CSpotLight_bu();
	virtual ~CSpotLight_bu() override {}
};