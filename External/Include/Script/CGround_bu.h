#pragma once
#include <Engine\CScript.h>
class CGround_bu : public CScript
{
public:
	void Start() override;
	void Update() override;

public:


public:
	CLONE(CGround_bu);
	CGround_bu();
	virtual ~CGround_bu() override;
};