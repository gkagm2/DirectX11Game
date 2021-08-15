#pragma once
#include "CModuleScript_ca.h"
class CBoosterScript_ca : public CModuleScript_ca
{
private:
	float m_fForce;		// ร฿ม๘ทย

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	float GetForce() { return m_fForce; }
	void SetForce(float _fForce) { m_fForce = _fForce; }
	
public:
	virtual void InitModule(E_ModuleLevel_ca _eLevel) override;

public:
	CLONE(CBoosterScript_ca);
	CBoosterScript_ca();
	virtual ~CBoosterScript_ca() override;
};