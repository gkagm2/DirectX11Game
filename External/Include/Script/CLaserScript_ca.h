#pragma once
#include "CPartsScript_ca.h"


struct TGriderInfo_ca : public TPartsInfo {

};

class CLaserScript_ca : public CPartsScript_ca
{
private:
	float fDamage;		// 데미지
	float fFireRate;	// 연사 속도

public:
	virtual void Start() override;
	virtual void Update() override;
public:
	CLONE(CLaserScript_ca);
	CLaserScript_ca();
	virtual ~CLaserScript_ca() override;
};

