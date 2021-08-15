#pragma once
#include "CModuleScript_ca.h"
class CCommandModuleScript_ca : public CModuleScript_ca
{
public:
	virtual void Start() override;
	virtual void Update() override;

public:
	virtual void InitModule(E_ModuleLevel_ca _eLevel) override;
public:
	CLONE(CCommandModuleScript_ca);
	CCommandModuleScript_ca();
	virtual ~CCommandModuleScript_ca() override;
};