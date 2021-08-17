#pragma once
#include <Engine\CScript.h>
class CKeyControllerScript_ca : public CScript
{
public:
	virtual void Start() override;
	virtual void Update() override; 
public:
	CLONE(CKeyControllerScript_ca);
	CKeyControllerScript_ca();
	virtual ~CKeyControllerScript_ca() override;
};