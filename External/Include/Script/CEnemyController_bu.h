#pragma once
#include <Engine\CScript.h>
class CEnemyController_bu : public CScript
{
public:
	virtual void Start() override;
	virtual void Update() override;

public:
	CLONE(CEnemyController_bu);
	CEnemyController_bu();
	virtual ~CEnemyController_bu() override;
};