#pragma once
#include <Engine\CCamera.h>
class CCameraEx : public CCamera
{
public:
	virtual void FinalUpdate() override;

public:
	CCameraEx();
	virtual ~CCameraEx() override;
};