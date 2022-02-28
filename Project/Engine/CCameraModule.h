#pragma once
#include "CCamera.h"
class CCameraModule : public CCamera
{
public:
	virtual void FinalUpdate() override;

public:
	CLONE(CCameraModule);
	CCameraModule();
	virtual ~CCameraModule() override;
};