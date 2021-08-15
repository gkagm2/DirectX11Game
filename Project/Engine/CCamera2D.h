#pragma once
#include "CCamera.h"
class CCamera2D : public CCamera
{
public:
	virtual const Vector2& GetScreenToWorldPosition() override;
public:
	CLONE(CCamera2D);
	CCamera2D();
	virtual ~CCamera2D() override;
};