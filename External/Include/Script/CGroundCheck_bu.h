#pragma once
#include <Engine\CScript.h>
class CGroundCheck_bu : public CScript
{
public:
	bool m_bTouchGround;
public:
	virtual void OnCollisionEnter2D(CCollider2D* _pCol) override;
	virtual void OnCollisionStay2D(CCollider2D* _pCol) override;
	virtual void OnCollisionExit2D(CCollider2D* _pCol) override;

	bool IsTouchGround() { return m_bTouchGround; }


	CLONE(CGroundCheck_bu);
	CGroundCheck_bu();
	virtual ~CGroundCheck_bu() override {}
};