#pragma once
#include "CModuleScript_ca.h"
class CGirderScript_ca : public CModuleScript_ca
{
public:
	virtual void Start() override;
	virtual void Update() override;

public:
	CLONE(CGirderScript_ca);
	CGirderScript_ca();
	virtual ~CGirderScript_ca() override;
};