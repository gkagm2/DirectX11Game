#pragma once
#include "CRigidbody.h"
class CRigidbody2D : public CRigidbody
{
public:
	virtual void PrevUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

public:
	CLONE(CRigidbody2D);
	CRigidbody2D();
	virtual ~CRigidbody2D() override;
	friend class CCollisionManager;
};