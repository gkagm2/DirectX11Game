#pragma once
#include <Engine\CScript.h>
class CDraggable_bu : public CScript
{
	bool m_bisMouseIn;
public:
	virtual void OnCollisionStay2D(CCollider2D* _pOther) override;
public:
	CLONE(CDraggable_bu);
	CDraggable_bu();
	virtual ~CDraggable_bu() override;
};