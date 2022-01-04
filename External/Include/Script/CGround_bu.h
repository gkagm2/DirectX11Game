#pragma once
#include <Engine\CScript.h>
class CGround_bu : public CScript
{
public:
	virtual void Start() override;
	virtual void Update() override;

public:
	CLONE(CGround_bu);
	CGround_bu();
	virtual ~CGround_bu() override;
};