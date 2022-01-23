#pragma once
#include <Engine\CScript.h>
class CChainsaw_bu : public CScript
{
private:
	float m_fDamaged;
public:
	virtual void OnCollisionEnter2D(CCollider2D* _pCol) override;

public:
	CLONE(CChainsaw_bu);
	CChainsaw_bu();
	virtual ~CChainsaw_bu() override {}
};