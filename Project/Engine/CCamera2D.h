#pragma once
#include "CCamera.h"
class CCamera2D : public CCamera
{
public:
	CLONE(CCamera2D);
	CCamera2D();
	virtual ~CCamera2D() override;
};