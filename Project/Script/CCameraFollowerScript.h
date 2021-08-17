#pragma once
#include <Engine\CScript.h>
class CCameraFollowerScript : public CScript
{
private:
	CGameObject* m_pTargetCamObj;
public:
	virtual void Start() override;
	virtual void Update() override;
public:
	CLONE(CCameraFollowerScript);
	CCameraFollowerScript();
	virtual ~CCameraFollowerScript() override;
};