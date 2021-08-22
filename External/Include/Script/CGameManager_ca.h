#pragma once
#include <Engine\CScript.h>
class CGameManager_ca : public CScript
{
public:
	virtual void Start() override;
	virtual void Update() override;

public:
	CLONE(CGameManager_ca);
	CGameManager_ca();
	virtual ~CGameManager_ca() override;
};