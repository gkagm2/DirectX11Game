#pragma once
#include <Engine\CScript.h>
class CLookAtScript : public CScript
{
public:
	virtual void Start() override;
	virtual void Update() override;
public:
	CLONE(CLookAtScript);
	CLookAtScript();
	virtual ~CLookAtScript() override;
};