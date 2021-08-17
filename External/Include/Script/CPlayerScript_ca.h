#pragma once
#include <Engine\CScript.h>
class CPlayerScript_ca : public CScript
{
public:
	virtual void Start() override;
	virtual void Update() override;

public:

public:
	CLONE(CPlayerScript_ca);
	CPlayerScript_ca();
	virtual ~CPlayerScript_ca() override;
};