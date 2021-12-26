#pragma once
#include <Engine\CCamera.h>
class CCameraClient : public CCamera
{
public:
	virtual void FinalUpdate() override;

public:
	CCameraClient();
	virtual ~CCameraClient() override;
};