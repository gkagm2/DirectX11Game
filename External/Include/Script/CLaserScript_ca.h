#pragma once
#include "CModuleScript_ca.h"

struct TGriderInfo_ca : public TPartsInfo {

};

class CLaserScript_ca : public CModuleScript_ca
{
private:
	float fDamage;		// ������
	float fFireRate;	// ���� �ӵ�

public:
	virtual void Start() override;
	virtual void Update() override;
public:
	CLONE(CLaserScript_ca);
	CLaserScript_ca();
	virtual ~CLaserScript_ca() override;
};

