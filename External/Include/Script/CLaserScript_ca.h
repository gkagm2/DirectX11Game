#pragma once
#include "CModuleScript_ca.h"

class CLaserScript_ca : public CModuleScript_ca
{
private:
	float m_fDamage;	// ������
	float m_fFireRate;	// ���� �ӵ�

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void SetDamage(float _fDamage) { m_fDamage = _fDamage; }
	float GetDamage() { return m_fDamage;}
	void SetFireRate(float _fFireRate) { m_fFireRate = _fFireRate; }
	float GetFireRate() { return m_fFireRate; }

public:
	virtual void _InitModuleLevel(E_ModuleLevel_ca _eLevel) override;

public:
	CLONE(CLaserScript_ca);
	CLaserScript_ca();
	virtual ~CLaserScript_ca() override;
};

