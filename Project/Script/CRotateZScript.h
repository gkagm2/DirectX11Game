#pragma once
#include <Engine\CScript.h>
class CRotateZScript : public CScript
{
private:
	float m_fSpeed;
public:
	virtual void Update() override;

public:
	CLONE(CRotateZScript);
	CRotateZScript();
	virtual ~CRotateZScript() override;
};