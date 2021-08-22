#pragma once
#include <Engine\CScript.h>
class CCameraFollowerScript : public CScript
{
private:
	CGameObject* m_pTargetObj;

	float m_fMouseScrollSpeed;
public:
	virtual void Awake() override;
	virtual void Update() override;

public:
	void Zoom();

public:
	CLONE(CCameraFollowerScript);
	CCameraFollowerScript();
	virtual ~CCameraFollowerScript() override;
};