#pragma once
#include "CCollider2D.h"
class CCollider2DRect : public CCollider2D
{
public:
	virtual void OnCollisionEnter(CCollider2D* pOther) override;
	virtual void OnCollisionStay(CCollider2D* pOther) override;
	virtual void OnCollisionExit(CCollider2D* pOther) override;
public:
	CCollider2DRect();
	virtual ~CCollider2DRect() override;
};