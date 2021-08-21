#pragma once
#include "CCollider2D.h"
class CCollider2DRect : public CCollider2D
{
public:
	virtual void OnCollisionEnter2D(CCollider2D* _pOther) override;
	virtual void OnCollisionStay2D(CCollider2D* _pOther) override;
	virtual void OnCollisionExit2D(CCollider2D* _pOther) override;
public:
	CLONE(CCollider2DRect);
	CCollider2DRect();
	virtual ~CCollider2DRect() override;
};