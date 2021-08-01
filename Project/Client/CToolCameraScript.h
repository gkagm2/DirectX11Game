#pragma once
#include <Engine\CScript.h>
class CToolCameraScript : public CScript
{
private:
	float m_fSpeed;
public:
	virtual void Update() override;

public:
	CLONE(CToolCameraScript);
	CToolCameraScript();
	virtual ~CToolCameraScript() override;
};