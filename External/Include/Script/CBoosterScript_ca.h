#pragma once
#include "CPartsScript_ca.h"
class CBoosterScript_ca : public CPartsScript_ca
{
private:
	float fForce;		// ������

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	CLONE(CBoosterScript_ca);
	CBoosterScript_ca();
	virtual ~CBoosterScript_ca() override;
};