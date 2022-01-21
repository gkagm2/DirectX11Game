#pragma once
#include <Engine\CScript.h>
class CRigidbody2DTestScript : public CScript
{
public:
	float fSpeed;

	bool deltaType;

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	CLONE(CRigidbody2DTestScript);
	CRigidbody2DTestScript();
	virtual ~CRigidbody2DTestScript() override{}
};