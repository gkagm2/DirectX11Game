#pragma once
#include <Engine\CScript.h>
class CToolCameraScript : public CScript
{
private:
	float m_fSpeed;

	Vector2 m_vPrevPos;
	Vector2 m_vMoveOffset;
public:
	virtual void Update() override;

public:
	CLONE(CToolCameraScript);
	CToolCameraScript();
	virtual ~CToolCameraScript() override;
};